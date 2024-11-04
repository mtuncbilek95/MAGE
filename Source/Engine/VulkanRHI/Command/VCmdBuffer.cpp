#include "VCmdBuffer.h"

#include "../Core/VAssert.h"
#include "../Core/VLoadFuncs.h"
#include "../Device/VDevice.h"
#include "../Command/VCmdPool.h"
#include "../Framebuffer/VFramebuffer.h"
#include "../RenderPass/VRenderPass.h"
#include "../Pipeline/VPipeline.h"
#include "../Buffer/VStageBuffer.h"
#include "../Buffer/VDstBuffer.h"
#include "../Image/VImage.h"
#include "../Image/VImageView.h"
#include "../Descriptor/VDescBuffer.h"
#include "../Descriptor/VDescSet.h"

namespace MAGE
{
	VCmdBuffer::VCmdBuffer(const CmdBufferProps& desc, VDevice* device) : VObject(device), m_props(desc), m_cmdBuffer(VK_NULL_HANDLE)
	{
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = desc.m_pool->GetCmdPool();
		allocInfo.level = desc.m_cmdLevel;
		allocInfo.commandBufferCount = 1;

		ErrorUtils::VkAssert(vkAllocateCommandBuffers(m_rootDevice->GetDevice(), &allocInfo, &m_cmdBuffer), "VCmdBuffer");
	}

	VCmdBuffer::~VCmdBuffer()
	{
		Destroy();
	}

	void VCmdBuffer::BeginRecording(VkCommandBufferUsageFlags useFlag) const
	{
		ErrorUtils::VkAssert(vkResetCommandBuffer(m_cmdBuffer, 0), "VCmdBuffer");

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = useFlag;
		beginInfo.pInheritanceInfo = nullptr;

		ErrorUtils::VkAssert(vkBeginCommandBuffer(m_cmdBuffer, &beginInfo), "VCmdBuffer");
	}

	void VCmdBuffer::BeginRecording(const RecordingProps& recordProp) const
	{
		ErrorUtils::VkAssert(vkResetCommandBuffer(m_cmdBuffer, 0), "VCmdBuffer");

		VkCommandBufferInheritanceInfo inherit = {};
		inherit.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
		inherit.renderPass = nullptr;
		inherit.framebuffer = nullptr;
		inherit.subpass = recordProp.subPass;
		inherit.occlusionQueryEnable = VK_FALSE;

		VkCommandBufferInheritanceRenderingInfo rendering = {};
		if (!recordProp.onlyTransfer)
		{
			rendering.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_RENDERING_INFO;
			rendering.colorAttachmentCount = recordProp.colorAttachments.size();
			rendering.pColorAttachmentFormats = recordProp.colorAttachments.data();
			rendering.depthAttachmentFormat = recordProp.depthAttachment;
			rendering.stencilAttachmentFormat = recordProp.stencilAttachment;
			rendering.viewMask = recordProp.viewMask;
			rendering.rasterizationSamples = recordProp.sampleCount;

			inherit.pNext = &rendering;
		}

		VkCommandBufferBeginInfo begin = {};
		begin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		begin.flags = recordProp.beginFlag;
		begin.pInheritanceInfo = &inherit;

		ErrorUtils::VkAssert(vkBeginCommandBuffer(m_cmdBuffer, &begin), "VCmdBuffer");
	}

	void VCmdBuffer::EndRecording() const
	{
		ErrorUtils::VkAssert(vkEndCommandBuffer(m_cmdBuffer), "VCmdBuffer");
	}

	void VCmdBuffer::BeginRenderPass(VRenderPass* renderPass, VFramebuffer* framebuffer, VkSubpassContents passFlag) const
	{
		VkRenderPassBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		beginInfo.renderPass = renderPass->GetRenderPass();
		beginInfo.framebuffer = framebuffer->GetBuffer();
		beginInfo.renderArea.extent = { framebuffer->GetRenderArea().x, framebuffer->GetRenderArea().y };
		beginInfo.renderArea.offset = { 0, 0 };
		beginInfo.clearValueCount = 1;

		VkClearValue colorVal = {};
		colorVal.color.float32[0] = 0.1f;
		colorVal.color.float32[1] = 0.2f;
		colorVal.color.float32[2] = 0.3f;
		colorVal.color.float32[3] = 1.0f;

		beginInfo.pClearValues = &colorVal;

		vkCmdBeginRenderPass(m_cmdBuffer, &beginInfo, passFlag);
	}

	void VCmdBuffer::EndRenderPass() const
	{
		vkCmdEndRenderPass(m_cmdBuffer);
	}

	void VCmdBuffer::BeginRendering(const RenderingProps& renderProp) const
	{
		Vector<VkRenderingAttachmentInfo> colorAttachments;
		
		for (u32 i = 0; i < renderProp.colorAttachments.size(); i++)
		{
			RenderAttachment attach = renderProp.colorAttachments[i];

			VkRenderingAttachmentInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
			info.imageView = attach.view->GetView();
			info.imageLayout = attach.layout;
			info.loadOp = attach.loadOp;
			info.storeOp = attach.storeOp;
			info.clearValue.color = { attach.colorValue.r, attach.colorValue.g, attach.colorValue.b, attach.colorValue.a };
		}

		VkRenderingAttachmentInfo depthAttachment = {};
		if (renderProp.hasDepth)
		{
			VkRenderingAttachmentInfo depthAttachment = {};
			depthAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
			depthAttachment.imageView = renderProp.depthAttachment.view->GetView();
			depthAttachment.imageLayout = renderProp.depthAttachment.layout;
			depthAttachment.loadOp = renderProp.depthAttachment.loadOp;
			depthAttachment.storeOp = renderProp.depthAttachment.storeOp;
			depthAttachment.clearValue.depthStencil.depth = renderProp.depthAttachment.depthValue.x;
		}

		VkRenderingAttachmentInfo stencilAttachment = {};
		if (renderProp.hasStencil)
		{
			VkRenderingAttachmentInfo depthAttachment = {};
			depthAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
			depthAttachment.imageView = renderProp.stencilAttachment.view->GetView();
			depthAttachment.imageLayout = renderProp.stencilAttachment.layout;
			depthAttachment.loadOp = renderProp.stencilAttachment.loadOp;
			depthAttachment.storeOp = renderProp.stencilAttachment.storeOp;
			depthAttachment.clearValue.depthStencil.stencil = static_cast<u32>(renderProp.stencilAttachment.depthValue.y);
		}

		VkRenderingInfo renderInfo = {};
		renderInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
		renderInfo.colorAttachmentCount = colorAttachments.size();
		renderInfo.pColorAttachments = colorAttachments.size() == 0 ? nullptr : colorAttachments.data();
		renderInfo.pDepthAttachment = renderProp.hasDepth ? &depthAttachment : nullptr;
		renderInfo.pStencilAttachment = renderProp.hasDepth ? &stencilAttachment : nullptr;
		renderInfo.renderArea.extent = { renderProp.extent.x, renderProp.extent.y };
		renderInfo.renderArea.offset = { renderProp.offset.x, renderProp.offset.y };
		renderInfo.viewMask = renderProp.viewMask;
		renderInfo.layerCount = renderProp.layerCount;

		vkCmdBeginRendering(m_cmdBuffer, &renderInfo);
	}

	void VCmdBuffer::EndRendering()
	{
		vkCmdEndRendering(m_cmdBuffer);
	}

	void VCmdBuffer::DrawIndexed(u32 indexCount, u32 indexOffset, u32 vertexOffset, u32 instanceOffset, u32 instanceCount) const
	{
		vkCmdDrawIndexed(m_cmdBuffer, indexCount, instanceCount, 0, vertexOffset, 0);
	}

	void VCmdBuffer::ExecuteCommand(VCmdBuffer* buffer) const
	{
		VkCommandBuffer cmd = buffer->GetCmdBuffer();

		vkCmdExecuteCommands(m_cmdBuffer, 1, &cmd);
	}

	void VCmdBuffer::ExecuteCommands(const Vector<VCmdBuffer*>& buffers) const
	{
		Vector<VkCommandBuffer> vkCmdBuffers;

		for (auto* buffer : buffers)
			vkCmdBuffers.push_back(buffer->GetCmdBuffer());

		vkCmdExecuteCommands(m_cmdBuffer, vkCmdBuffers.size(), vkCmdBuffers.data());
	}

	void VCmdBuffer::BindPipeline(VPipeline* pipeline)
	{
		vkCmdBindPipeline(m_cmdBuffer, pipeline->GetBindPoint(), pipeline->GetPipeline());
		boundPipeline = pipeline;
	}

	void VCmdBuffer::BindDynamicState(VkViewport viewport, VkRect2D scissor) const
	{
	}

	void VCmdBuffer::BindVertexBuffer(VDstBuffer* vertexBuffer) const
	{
		u64 offset = 0;
		VkBuffer vBuffer = vertexBuffer->GetBuffer();
		vkCmdBindVertexBuffers(m_cmdBuffer, 0, 1, &vBuffer, &offset);
	}

	void VCmdBuffer::BindIndexBuffer(VDstBuffer* indexBuffer, u32 offset) const
	{
		vkCmdBindIndexBuffer(m_cmdBuffer, indexBuffer->GetBuffer(), offset, VK_INDEX_TYPE_UINT32);
	}

	void VCmdBuffer::BindDescriptorBuffer(VDescBuffer* dscBuffer) const
	{
		VkDescriptorBufferBindingInfoEXT info = {};
		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_BUFFER_BINDING_INFO_EXT;
		info.usage = VK_BUFFER_USAGE_RESOURCE_DESCRIPTOR_BUFFER_BIT_EXT;
		info.address = dscBuffer->GetAddress();
		BindDescriptorEXT(m_cmdBuffer, 1, &info);

		u32 index = 0;
		u64 offset = dscBuffer->GetOffset();
		SetDescriptorBufferOffsetEXT(m_cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, boundPipeline->GetLayout(), 0, 1, &index, &offset);
	}

	void VCmdBuffer::BindDescriptorSet(VDescSet* descSet)
	{
		VkDescriptorSet set = descSet->GetSet();
		vkCmdBindDescriptorSets(m_cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, boundPipeline->GetLayout(), 0, 1, &set, 0, 0);
	}

	void VCmdBuffer::CopyBufferToBuffer(VStageBuffer* srcBuffer, VDstBuffer* dstBuffer) const
	{
		VkBufferCopy copyInfo = {};
		copyInfo.dstOffset = 0;
		copyInfo.size = dstBuffer->GetTotalSize();
		copyInfo.srcOffset = 0;

		vkCmdCopyBuffer(m_cmdBuffer, srcBuffer->GetBuffer(), dstBuffer->GetBuffer(), 1, &copyInfo);
	}

	void VCmdBuffer::CopyBufferToImage(VStageBuffer* srcBuffer, VImage* dstBuffer) const
	{
		VkBufferImageCopy regionData = {};
		regionData.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		regionData.imageSubresource.baseArrayLayer = 0;
		regionData.imageSubresource.layerCount = 1;
		regionData.imageSubresource.mipLevel = 0;
		regionData.imageExtent = { dstBuffer->GetImageSize().x, dstBuffer->GetImageSize().y, dstBuffer->GetImageSize().z };
		regionData.bufferImageHeight = 0;
		regionData.bufferOffset = 0;
		regionData.bufferRowLength = 0;
		regionData.imageOffset = { 0, 0, 0 };
		vkCmdCopyBufferToImage(m_cmdBuffer, srcBuffer->GetBuffer(), dstBuffer->GetImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &regionData);
	}

	void VCmdBuffer::TranslateImageBarrier(const ImageBarrierProps& barrierInfo) const
	{
		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.image = barrierInfo.image->GetImage();
		barrier.srcAccessMask = barrierInfo.srcAccessMask;
		barrier.dstAccessMask = barrierInfo.dstAccessMask;
		barrier.oldLayout = barrierInfo.oldLayout;
		barrier.newLayout = barrierInfo.newLayout;
		barrier.srcQueueFamilyIndex = barrierInfo.srcQueueFamilyIndex;
		barrier.dstQueueFamilyIndex = barrierInfo.dstQueueFamilyIndex;

		barrier.subresourceRange.aspectMask = barrierInfo.aspectMask;
		barrier.subresourceRange.baseMipLevel = barrierInfo.baseMipLevel;
		barrier.subresourceRange.levelCount = barrierInfo.levelCount;
		barrier.subresourceRange.baseArrayLayer = barrierInfo.baseArrayLayer;
		barrier.subresourceRange.layerCount = barrierInfo.layerCount;

		vkCmdPipelineBarrier(m_cmdBuffer, barrierInfo.srcPipeline, barrierInfo.dstPipeline, 0, 0, nullptr, 0, nullptr, 1, &barrier);
	}

	void VCmdBuffer::Destroy()
	{
		if (m_cmdBuffer != VK_NULL_HANDLE)
		{
			vkFreeCommandBuffers(m_rootDevice->GetDevice(), m_props.m_pool->GetCmdPool(), 1, &m_cmdBuffer);
			m_cmdBuffer = VK_NULL_HANDLE;
		}
	}
}
