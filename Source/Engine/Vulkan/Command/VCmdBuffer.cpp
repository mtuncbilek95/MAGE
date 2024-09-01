#include "VCmdBuffer.h"
#include "VCmdUtils.h"
#include "Engine/Vulkan/Core/VCoreUtils.h"
#include "Engine/Vulkan/Shader/VShaderUtils.h"
#include "Engine/Vulkan/Descriptor/VDescriptorUtils.h"
#include "Engine/Vulkan/Pipeline/VPipelineUtils.h"
#include "Engine/Vulkan/Texture/VTextureUtils.h"
#include "Engine/Vulkan/Buffer/VBufferUtils.h"
#include "Engine/Vulkan/Memory/VMemoryUtils.h"

#include "Engine/Vulkan/Device/VDevice.h"
#include "Engine/Vulkan/Command/VCmdPool.h"
#include "Engine/Vulkan/Texture/VTextureImage.h"
#include "Engine/Vulkan/Texture/VTextureView.h"
#include "Engine/Vulkan/Pipeline/VPipeline.h"
#include "Engine/Vulkan/Buffer/VBuffer.h"
#include "Engine/Vulkan/Descriptor/VDescriptorSet.h"

#include "Engine/Graphics/API/GraphicsAPI.h"

#include "Engine/Platform/PlatformErrorMessage.h"

namespace MAGE
{
	VCmdBuffer::VCmdBuffer(const CmdBufferDesc& desc, VDevice* pDevice) : CmdBuffer(desc, pDevice),
		mDevice(pDevice->GetVkDevice()), mVkCmdBuffer(VK_NULL_HANDLE)
	{
		mVkCmdPool = desc.pOwnerPool->GetAs<VCmdPool>()->GetVkCommandPool();

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = mVkCmdPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		MAGE_ASSERT(vkAllocateCommandBuffers(mDevice, &allocInfo, &mVkCmdBuffer) == VK_SUCCESS, "VCmdBuffer", "Failed to allocate command buffer");
	}

	void VCmdBuffer::BeginRecordingImpl()
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		beginInfo.pInheritanceInfo = nullptr;

		MAGE_ASSERT(vkBeginCommandBuffer(mVkCmdBuffer, &beginInfo) == VK_SUCCESS, "VCmdBuffer", "Failed to begin recording command buffer");
	}

	void VCmdBuffer::EndRecordingImpl()
	{
		MAGE_ASSERT(vkEndCommandBuffer(mVkCmdBuffer) == VK_SUCCESS, "VCmdBuffer", "Failed to end recording command buffer");
	}

	void VCmdBuffer::BeginRenderingImpl(const DynamicPassDesc& desc)
	{
		Vector<VkRenderingAttachmentInfo> attachmentInfos;

		for (u32 i = 0; i < desc.ColorAttachments.size(); i++)
		{
			VkRenderingAttachmentInfo attachment = {};
			attachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
			attachment.imageView = desc.ColorAttachments[i].ImageBuffer->GetAs<VTextureView>()->GetVkTextureView();
			attachment.imageLayout = VkUtils::GetVkImageLayout(desc.ColorAttachments[i].ImageLayout);

			if (!desc.ColorAttachments[i].ResolveBuffer)
			{
				attachment.resolveMode = VK_RESOLVE_MODE_NONE_KHR;
				attachment.resolveImageView = VK_NULL_HANDLE;
				attachment.resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			}
			else
			{
				attachment.resolveMode = VkUtils::GetResolveMode(desc.ColorAttachments[i].ResolveMode);
				attachment.resolveImageView = desc.ColorAttachments[i].ResolveBuffer->GetAs<VTextureView>()->GetVkTextureView();
				attachment.resolveImageLayout = VkUtils::GetVkImageLayout(desc.ColorAttachments[i].ResolveLayout);
			}

			attachment.loadOp = VkUtils::GetVkAttachmentLoadOp(desc.ColorAttachments[i].LoadOperation);
			attachment.storeOp = VkUtils::GetVkAttachmentStoreOp(desc.ColorAttachments[i].StoreOperation);
			attachment.clearValue.color = { desc.ColorAttachments[i].ClearColor.x, desc.ColorAttachments[i].ClearColor.y, desc.ColorAttachments[i].ClearColor.z, desc.ColorAttachments[i].ClearColor.w };
			attachment.pNext = nullptr;

			attachmentInfos.push_back(attachment);
		}

		VkRenderingAttachmentInfo depthAttachment = {};
		if (desc.DepthAttachment.ImageBuffer)
		{
			depthAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
			depthAttachment.imageView = desc.DepthAttachment.ImageBuffer->GetAs<VTextureView>()->GetVkTextureView();
			depthAttachment.imageLayout = VkUtils::GetVkImageLayout(desc.DepthAttachment.ImageLayout);
			depthAttachment.loadOp = VkUtils::GetVkAttachmentLoadOp(desc.DepthAttachment.LoadOperation);
			depthAttachment.storeOp = VkUtils::GetVkAttachmentStoreOp(desc.DepthAttachment.StoreOperation);
			depthAttachment.clearValue.depthStencil = { desc.DepthAttachment.ClearDepthStencil.x, (u32)desc.DepthAttachment.ClearDepthStencil.y };
		}

		VkRenderingAttachmentInfo stencilAttachment = {};
		if (desc.StencilAttachment.ImageBuffer)
		{
			stencilAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
			stencilAttachment.imageView = desc.StencilAttachment.ImageBuffer->GetAs<VTextureView>()->GetVkTextureView();
			stencilAttachment.imageLayout = VkUtils::GetVkImageLayout(desc.StencilAttachment.ImageLayout);
			stencilAttachment.loadOp = VkUtils::GetVkAttachmentLoadOp(desc.StencilAttachment.LoadOperation);
			stencilAttachment.storeOp = VkUtils::GetVkAttachmentStoreOp(desc.StencilAttachment.StoreOperation);
			stencilAttachment.clearValue.depthStencil = { desc.StencilAttachment.ClearDepthStencil.x, (u32)desc.StencilAttachment.ClearDepthStencil.y };
		}

		VkRenderingInfoKHR  renderingInfo = {};
		renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR;
		renderingInfo.renderArea = { { desc.Offset.x, desc.Offset.y }, { desc.Extent.x, desc.Extent.y } };
		renderingInfo.layerCount = desc.layerCount;
		renderingInfo.viewMask = desc.viewMask;
		renderingInfo.colorAttachmentCount = attachmentInfos.size();
		renderingInfo.pColorAttachments = attachmentInfos.data();
		
		if(desc.DepthAttachment.ImageBuffer)
			renderingInfo.pDepthAttachment = &depthAttachment;
		if(desc.StencilAttachment.ImageBuffer)
			renderingInfo.pStencilAttachment = &stencilAttachment;
		
		renderingInfo.flags = VkRenderingFlags();
		renderingInfo.pNext = nullptr;

		vkCmdBeginRendering(mVkCmdBuffer, &renderingInfo);
	}

	void VCmdBuffer::EndRenderingImpl()
	{
		vkCmdEndRendering(mVkCmdBuffer);
	}

	void VCmdBuffer::BindPipelineImpl(Pipeline* pPipeline)
	{
		auto pVPipeline = pPipeline->GetAs<VPipeline>();
		vkCmdBindPipeline(mVkCmdBuffer, VkUtils::GetVkPipelineBindPoint(pVPipeline->GetBindPoint()), pVPipeline->GetVkPipeline());
	}

	void VCmdBuffer::BindVertexBuffersImpl(GraphicsBuffer* ppBuffer[], u32 amount)
	{
		VkBuffer buffers[16];
		VkDeviceSize offsets[16];

		for (u32 i = 0; i < amount; i++)
		{
			auto pVBuffer = ppBuffer[i]->GetAs<VBuffer>();
			buffers[i] = pVBuffer->GetBuffer();
			offsets[i] = 0;
		}

		vkCmdBindVertexBuffers(mVkCmdBuffer, 0, amount, buffers, offsets);
	}

	void VCmdBuffer::BindIndexBufferImpl(GraphicsBuffer* pBuffer, GraphicsIndexType indexType)
	{
		auto pVBuffer = pBuffer->GetAs<VBuffer>();
		vkCmdBindIndexBuffer(mVkCmdBuffer, pVBuffer->GetBuffer(), 0, VkUtils::GetIndexType(indexType));
	}

	void VCmdBuffer::BindDescriptorsImpl(DescriptorSet* ppSet[], u32 amount, u32 setIndex)
	{
		VkDescriptorSet sets[16];
		for (u32 i = 0; i < amount; i++)
		{
			auto pVDescriptorSet = ppSet[i]->GetAs<VDescriptorSet>();
			sets[i] = pVDescriptorSet->GetVkDescriptorSet();
		}

		vkCmdBindDescriptorSets(mVkCmdBuffer, VkUtils::GetVkPipelineBindPoint(GetBoundPipeline()->GetBindPoint()),
			GetBoundPipeline()->GetAs<VPipeline>()->GetVkPipelineLayout(), setIndex, amount, sets, 0, nullptr);
	}

	void VCmdBuffer::PushConstantsImpl(MemoryBuffer buffer, u32 offset, ShaderStage stage)
	{
		auto pPipeline = GetBoundPipeline()->GetAs<VPipeline>();
		vkCmdPushConstants(mVkCmdBuffer, pPipeline->GetVkPipelineLayout(), VkUtils::GetVkShaderStage(stage), offset, buffer.Size(), buffer.Data());
	}

	void VCmdBuffer::DrawIndexedImpl(u32 indexCount, u32 indexOffset, u32 vertexOffset, u32 instanceOffset, u32 instanceCount)
	{
		vkCmdDrawIndexed(mVkCmdBuffer, indexCount, instanceCount, indexOffset, vertexOffset, instanceOffset);
	}

	void VCmdBuffer::SetViewportsImpl(ViewportDesc pViewports[], u8 count)
	{
		VkViewport viewports[32];
		for (u8 i = 0; i < count; i++)
		{
			viewports[i].x = pViewports[i].OffsetSize.x;
			viewports[i].y = pViewports[i].OffsetSize.y;
			viewports[i].width = pViewports[i].ViewportSize.x;
			viewports[i].height = pViewports[i].ViewportSize.y;
			viewports[i].minDepth = pViewports[i].DepthRange.x;
			viewports[i].maxDepth = pViewports[i].DepthRange.y;
		}

		vkCmdSetViewport(mVkCmdBuffer, 0, count, viewports);
	}

	void VCmdBuffer::SetScissorsImpl(ScissorDesc pScissors[], u8 count)
	{
		VkRect2D scissors[32];
		for (u8 i = 0; i < count; i++)
		{
			scissors[i].offset = { (i32)pScissors[i].OffsetSize.x, (i32)pScissors[i].OffsetSize.y };
			scissors[i].extent = { pScissors[i].ScissorSize.x, pScissors[i].ScissorSize.y };
		}

		vkCmdSetScissor(mVkCmdBuffer, 0, count, scissors);
	}

	void VCmdBuffer::ExecuteCachedCommandImpl(CmdBuffer* cmdBuffers[], u32 count)
	{
		VkCommandBuffer vkCmdBuffers[16];
		for (u32 i = 0; i < count; i++)
		{
			auto pVCmdBuffer = cmdBuffers[i]->GetAs<VCmdBuffer>();
			vkCmdBuffers[i] = pVCmdBuffer->GetVkCmdBuffer();
		}

		vkCmdExecuteCommands(mVkCmdBuffer, count, vkCmdBuffers);
	}

	void VCmdBuffer::CopyBufferToBufferImpl(GraphicsBuffer* pSourceBuffer, GraphicsBuffer* pDestinationBuffer, BufferBufferCopyDesc& desc)
	{
		auto pVSourceBuffer = pSourceBuffer->GetAs<VBuffer>();
		auto pVDestinationBuffer = pDestinationBuffer->GetAs<VBuffer>();

		VkBufferCopy copyRegion = {};
		copyRegion.srcOffset = desc.SourceOffset;
		copyRegion.dstOffset = desc.DestinationOffset;
		copyRegion.size = desc.Size;

		vkCmdCopyBuffer(mVkCmdBuffer, pVSourceBuffer->GetBuffer(), pVDestinationBuffer->GetBuffer(), 1, &copyRegion);
	}

	void VCmdBuffer::CopyBufferToTextureImpl(GraphicsBuffer* pSourceBuffer, TextureImage* pDestinationTexture, BufferTextureCopyDesc& desc)
	{
		auto srcBuffer = pSourceBuffer->GetAs<VBuffer>();
		auto dstImage = pDestinationTexture->GetAs<VTextureImage>();

		VkBufferImageCopy copyRegion = {};
		copyRegion.bufferOffset = desc.BufferOffsetInBytes;
		copyRegion.bufferRowLength = 0;
		copyRegion.bufferImageHeight = 0;

		copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		copyRegion.imageSubresource.mipLevel = desc.TargetMipIndex;
		copyRegion.imageSubresource.baseArrayLayer = desc.TargetArrayIndex;
		copyRegion.imageSubresource.layerCount = dstImage->GetArrayLayers();

		copyRegion.imageOffset = { desc.TextureOffset.x, desc.TextureOffset.y, desc.TextureOffset.z };
		copyRegion.imageExtent = { desc.TextureSize.x, desc.TextureSize.y, desc.TextureSize.z };

		vkCmdCopyBufferToImage(mVkCmdBuffer, srcBuffer->GetBuffer(), dstImage->GetVkTexture(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);
	}

	void VCmdBuffer::SetTextureBarrierImpl(TextureImage* pTexture, TextureImageBarrier& desc)
	{
		auto pVTexture = pTexture->GetAs<VTextureImage>();
		auto pDevice = GetOwnerDevice()->GetAs<VDevice>();
		u32 srcIndex;
		u32 dstIndex;

		switch (desc.SourceQueue)
		{
		case GraphicsQueueType::Graphics:
			srcIndex = pDevice->GetGraphicsQueueFamilyIndex();
			break;
		case GraphicsQueueType::Compute:
			srcIndex = pDevice->GetComputeQueueFamilyIndex();
			break;
		case GraphicsQueueType::Transfer:
			srcIndex = pDevice->GetTransferQueueFamilyIndex();
			break;
		default:
			srcIndex = VK_QUEUE_FAMILY_IGNORED;
			break;
		}

		switch (desc.DestinationQueue)
		{
		case GraphicsQueueType::Graphics:
			dstIndex = pDevice->GetGraphicsQueueFamilyIndex();
			break;
		case GraphicsQueueType::Compute:
			dstIndex = pDevice->GetComputeQueueFamilyIndex();
			break;
		case GraphicsQueueType::Transfer:
			dstIndex = pDevice->GetTransferQueueFamilyIndex();
			break;
		default:
			dstIndex = VK_QUEUE_FAMILY_IGNORED;
			break;
		}

		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = VkUtils::GetVkImageLayout(desc.OldLayout);
		barrier.newLayout = VkUtils::GetVkImageLayout(desc.NewLayout);
		barrier.srcQueueFamilyIndex = srcIndex;
		barrier.dstQueueFamilyIndex = dstIndex;
		barrier.image = pVTexture->GetVkTexture();
		barrier.subresourceRange.aspectMask = VkUtils::GetVkTextureAspectFlags(desc.AspectMask);
		barrier.subresourceRange.baseMipLevel = desc.MipIndex;
		barrier.subresourceRange.levelCount = pTexture->GetMipLevels();
		barrier.subresourceRange.baseArrayLayer = desc.ArrayIndex;
		barrier.subresourceRange.layerCount = pTexture->GetArrayLayers();
		barrier.srcAccessMask = VkUtils::GetVkAccessFlags(desc.SourceAccessMask);
		barrier.dstAccessMask = VkUtils::GetVkAccessFlags(desc.DestinationAccessMask);

		vkCmdPipelineBarrier(mVkCmdBuffer, VkUtils::GetVkPipelineStageFlags(desc.SourceStageFlags), VkUtils::GetVkPipelineStageFlags(desc.DestinationStageFlags), 0, 0, nullptr, 0, nullptr, 1, &barrier);
	}

	void VCmdBuffer::SetBufferBarrierImpl(GraphicsBuffer* pBuffer, BufferMemoryBarrier& desc)
	{
		auto pVBuffer = pBuffer->GetAs<VBuffer>();
		auto pDevice = GetOwnerDevice()->GetAs<VDevice>();
		u32 srcIndex;
		u32 dstIndex;

		switch (desc.SourceQueue)
		{
		case GraphicsQueueType::Graphics:
			srcIndex = pDevice->GetGraphicsQueueFamilyIndex();
			break;
		case GraphicsQueueType::Compute:
			srcIndex = pDevice->GetComputeQueueFamilyIndex();
			break;
		case GraphicsQueueType::Transfer:
			srcIndex = pDevice->GetTransferQueueFamilyIndex();
			break;
		default:
			srcIndex = VK_QUEUE_FAMILY_IGNORED;
			break;
		}

		switch (desc.DestinationQueue)
		{
		case GraphicsQueueType::Graphics:
			dstIndex = pDevice->GetGraphicsQueueFamilyIndex();
			break;
		case GraphicsQueueType::Compute:
			dstIndex = pDevice->GetComputeQueueFamilyIndex();
			break;
		case GraphicsQueueType::Transfer:
			dstIndex = pDevice->GetTransferQueueFamilyIndex();
			break;
		default:
			dstIndex = VK_QUEUE_FAMILY_IGNORED;
			break;
		}

		VkBufferMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
		barrier.srcAccessMask = VkUtils::GetVkAccessFlags(desc.SourceAccessMask);
		barrier.dstAccessMask = VkUtils::GetVkAccessFlags(desc.DestinationAccessMask);
		barrier.srcQueueFamilyIndex = srcIndex;
		barrier.dstQueueFamilyIndex = dstIndex;
		barrier.buffer = pVBuffer->GetBuffer();
		barrier.offset = desc.Offset;
		barrier.size = desc.Size;

		vkCmdPipelineBarrier(mVkCmdBuffer, VkUtils::GetVkPipelineStageFlags(desc.SourceStageFlags), VkUtils::GetVkPipelineStageFlags(desc.DestinationStageFlags), 0, 0, nullptr, 1, &barrier, 0, nullptr);
	}
}
