#include "VCmdBuffer.h"

#include "../Core/VkAssert.h"
#include "../Device/VDevice.h"
#include "../Command/VCmdPool.h"
#include "../Buffer/VBuffer.h"
#include "../Pipeline/VPipeline.h"
#include "../Descriptor/VDescSet.h"
#include "../Image/VImage.h"
#include "../Image/VImageView.h"

namespace MAGE
{
	VCmdBuffer::VCmdBuffer(const CmdBufferProps& desc, VDevice* device) : VkObject(device), m_props(desc), m_buffer(VK_NULL_HANDLE)
	{
		VkCommandBufferAllocateInfo allocInfo = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
		allocInfo.commandPool = desc.m_pool->GetVkCmdPool();
		allocInfo.level = desc.m_cmdLevel;
		allocInfo.commandBufferCount = 1;

		ErrorUtils::VkAssert(vkAllocateCommandBuffers(m_rootDevice->GetVkDevice(), &allocInfo, &m_buffer), "VCmdBuffer");
	}

	VCmdBuffer::~VCmdBuffer()
	{
		Destroy();
	}

	void VCmdBuffer::BeginRecord(VkCommandBufferUsageFlags flags) const
	{
		vkResetCommandBuffer(m_buffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);

		VkCommandBufferBeginInfo info = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
		info.flags = flags;

		ErrorUtils::VkAssert(vkBeginCommandBuffer(m_buffer, &info), "VCmdBUffer");
	}

	void VCmdBuffer::BeginRecord(const InheritanceProps& desc) const
	{
		vkResetCommandBuffer(m_buffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);

		VkCommandBufferInheritanceRenderingInfo rendering = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_RENDERING_INFO };
		rendering.colorAttachmentCount = desc.colorAttachments.size();
		rendering.pColorAttachmentFormats = desc.colorAttachments.data();
		rendering.depthAttachmentFormat = desc.depthAttachment;
		rendering.stencilAttachmentFormat = desc.stencilAttachment;
		rendering.flags = desc.renderFlags;
		rendering.viewMask = desc.viewMask;
		rendering.rasterizationSamples = desc.msaaCount;

		VkCommandBufferInheritanceInfo inheritance = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO };
		inheritance.framebuffer = nullptr;
		inheritance.renderPass = nullptr;
		inheritance.occlusionQueryEnable = false;
		inheritance.pNext = &rendering;

		VkCommandBufferBeginInfo info = { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
		info.flags = desc.usageFlags;
		info.pInheritanceInfo = &inheritance;

		ErrorUtils::VkAssert(vkBeginCommandBuffer(m_buffer, &info), "VCmdBUffer");
	}

	void VCmdBuffer::EndRecord() const
	{
		vkEndCommandBuffer(m_buffer);
	}

	void VCmdBuffer::BeginRendering(const RenderProps& desc) const
	{
		vector<VkRenderingAttachmentInfo> colors;

		for (u32 i = 0; i < desc.colorAttachment.size(); i++)
		{
			const RenderAttachment& att = desc.colorAttachment[i];

			VkRenderingAttachmentInfo info = { VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO };
			info.imageView = att.view->GetVkView();
			info.imageLayout = att.layout;
			info.loadOp = att.loadOp;
			info.storeOp = att.storeOp;
			info.clearValue.color = att.color;
			colors.push_back(info);
		}

		VkRenderingAttachmentInfo depth = { VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO };
		VkRenderingAttachmentInfo stencil = { VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO };
		if (desc.hasDepth)
		{
			depth.imageView = desc.depthAttachment.view->GetVkView();
			depth.imageLayout = desc.depthAttachment.layout;
			depth.loadOp = desc.depthAttachment.loadOp;
			depth.storeOp = desc.depthAttachment.storeOp;
			depth.clearValue.depthStencil = desc.depthAttachment.depthStencil;
		}

		if (desc.hasStencil)
		{
			stencil.imageView = desc.stencilAttachment.view->GetVkView();
			stencil.imageLayout = desc.stencilAttachment.layout;
			stencil.loadOp = desc.stencilAttachment.loadOp;
			stencil.storeOp = desc.stencilAttachment.storeOp;
			stencil.clearValue.depthStencil = desc.stencilAttachment.depthStencil;
		}

		VkRenderingInfo info = { VK_STRUCTURE_TYPE_RENDERING_INFO };
		info.colorAttachmentCount = colors.size();
		info.pColorAttachments = colors.data();
		info.pDepthAttachment = desc.hasDepth ? &depth : nullptr;
		info.pStencilAttachment = desc.hasStencil ? &stencil : nullptr;
		info.renderArea.extent = desc.extent;
		info.renderArea.offset = desc.offset;
		info.viewMask = desc.viewMask;
		info.layerCount = desc.layerCount;
		info.flags = desc.renderFlags;

		vkCmdBeginRendering(m_buffer, &info);
	}

	void VCmdBuffer::EndRendering() const
	{
		vkCmdEndRendering(m_buffer);
	}

	void VCmdBuffer::ImageBarrier(const ImageBarrierProps& desc) const
	{
		VkImageMemoryBarrier barrier = { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
		barrier.srcAccessMask = desc.srcAccessMask;
		barrier.dstAccessMask = desc.dstAccessMask;
		barrier.oldLayout = desc.oldLayout;
		barrier.newLayout = desc.newLayout;
		barrier.srcQueueFamilyIndex = desc.srcQueueFamilyIndex;
		barrier.dstQueueFamilyIndex = desc.dstQueueFamilyIndex;
		barrier.image = desc.image->GetVkImage();
		barrier.subresourceRange = VkImageSubresourceRange(desc.aspectMask, desc.baseMipLevel, desc.levelCount, desc.baseArrayLayer, desc.layerCount);

		vkCmdPipelineBarrier(m_buffer, desc.srcPipeline, desc.dstPipeline, {}, 0, nullptr, 0, nullptr, 1, &barrier);
	}

	void VCmdBuffer::BindPipeline(VPipeline* pipeline)
	{
		vkCmdBindPipeline(m_buffer, pipeline->GetVkBindPoint(), pipeline->GetVkPipeline());
		m_boundPipeline = pipeline;
	}

	void VCmdBuffer::BindDescriptors(const vector<VDescSet*>& sets) const
	{
		vector<VkDescriptorSet> descs(sets.size());

		for (u32 i = 0; i < descs.size(); i++)
			descs[i] = sets[i]->GetVkSet();

		u32 offset = 0;
		vkCmdBindDescriptorSets(m_buffer, m_boundPipeline->GetVkBindPoint(), m_boundPipeline->GetVkLayout(), 0, descs.size(), descs.data(), 0, &offset);
	}

	void VCmdBuffer::BindVertexBuffer(const vector<VBuffer*>& buffers) const
	{
		vector<VkBuffer> bf(buffers.size());
		usize offset = 0;
		for (u32 i = 0; i < buffers.size(); i++)
			bf[i] = buffers[i]->GetVkBuffer();

		vkCmdBindVertexBuffers(m_buffer, 0, bf.size(), bf.data(), &offset);
	}

	void VCmdBuffer::BindIndexBuffer(VBuffer* iBuffer, usize offset) const
	{
		vkCmdBindIndexBuffer(m_buffer, iBuffer->GetVkBuffer(), offset, VK_INDEX_TYPE_UINT32);
	}

	void VCmdBuffer::DrawIndexed(u32 indexCount, u32 indexOffset, u32 vertexOffset, u32 instanceOffset, u32 instanceCount) const
	{
		vkCmdDrawIndexed(m_buffer, indexCount, instanceCount, 0, vertexOffset, 0);
	}

	void VCmdBuffer::ExecuteCommands(const vector<VCmdBuffer*>& buffers) const
	{
		vector<VkCommandBuffer> cmds(buffers.size());
		for (u32 i = 0; i < cmds.size(); i++)
			cmds[i] = buffers[i]->GetVkCmdBuffer();

		vkCmdExecuteCommands(m_buffer, cmds.size(), cmds.data());
	}

	void VCmdBuffer::Destroy()
	{
		if (m_buffer != VK_NULL_HANDLE)
		{
			vkFreeCommandBuffers(m_rootDevice->GetVkDevice(), m_props.m_pool->GetVkCmdPool(), 1, &m_buffer);
			m_buffer = VK_NULL_HANDLE;
		}
	}
}
