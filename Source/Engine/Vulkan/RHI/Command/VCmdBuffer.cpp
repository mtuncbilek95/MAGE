#include "VCmdBuffer.h"

#include "../Device/VDevice.h"
#include "../Command/VCmdPool.h"
#include "../Image/VImageView.h"

#include "Engine/Vulkan/Core/VkAssert.h"

namespace MAGE
{
	VCmdBuffer::VCmdBuffer(const CmdBufferProps& desc, VDevice* device) : VkObject(device), m_props(desc)
	{
		vk::CommandBufferAllocateInfo info = {};
		info.commandBufferCount = 1;
		info.commandPool = desc.pool->GetVkCmdPool();
		info.level = desc.level;

		ErrorUtils::VkAssert(m_rootDevice->GetVkDevice().allocateCommandBuffers(&info, &m_buffer), "VCmdBuffer");
	}

	VCmdBuffer::~VCmdBuffer()
	{
		Destroy();
	}

	void VCmdBuffer::BeginRecord(vk::CommandBufferUsageFlags flags) const
	{
		m_buffer.reset(vk::CommandBufferResetFlagBits::eReleaseResources);

		vk::CommandBufferBeginInfo info = {};
		info.flags = flags;

		ErrorUtils::VkAssert(m_buffer.begin(&info), "VCmdBUffer");
	}

	void VCmdBuffer::BeginRecord(const InheritanceProps& desc) const
	{
		m_buffer.reset(vk::CommandBufferResetFlagBits::eReleaseResources);

		vk::CommandBufferInheritanceRenderingInfo rendering = {};
		rendering.colorAttachmentCount = desc.colorAttachments.size();
		rendering.pColorAttachmentFormats = desc.colorAttachments.data();
		rendering.depthAttachmentFormat = desc.depthAttachment;
		rendering.stencilAttachmentFormat = desc.stencilAttachment;
		rendering.flags = desc.renderFlags;
		rendering.viewMask = desc.viewMask;
		rendering.rasterizationSamples = desc.msaaCount;

		vk::CommandBufferInheritanceInfo inheritance = {};
		inheritance.framebuffer = nullptr;
		inheritance.renderPass = nullptr;
		inheritance.occlusionQueryEnable = false;
		inheritance.pNext = &rendering;

		vk::CommandBufferBeginInfo info = {};
		info.flags = desc.usageFlags;
		info.pInheritanceInfo = &inheritance;

		ErrorUtils::VkAssert(m_buffer.begin(&info), "VCmdBUffer");
	}

	void VCmdBuffer::EndRecord() const
	{
		m_buffer.end();
	}

	void VCmdBuffer::BeginRendering(const RenderProps& desc) const
	{
		Vector<vk::RenderingAttachmentInfoKHR> colorAttachments;

		for (u32 i = 0; i < desc.colorAttachment.size(); i++)
		{
			const RenderAttachment& att = desc.colorAttachment[i];

			vk::RenderingAttachmentInfoKHR info = vk::RenderingAttachmentInfo();
			info.imageView = att.view->GetVkView();
			info.imageLayout = att.layout;
			info.loadOp = att.loadOp;
			info.storeOp = att.storeOp;
			info.clearValue.setColor(att.color);
			colorAttachments.push_back(info);
		}

		vk::RenderingAttachmentInfoKHR depth = {};
		vk::RenderingAttachmentInfoKHR stencil = {};
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

		vk::RenderingInfoKHR info = {};
		info.colorAttachmentCount = colorAttachments.size();
		info.pColorAttachments = colorAttachments.data();
		info.pDepthAttachment = desc.hasDepth ? &depth : nullptr;
		info.pStencilAttachment = desc.hasStencil ? &stencil : nullptr;
		info.renderArea.extent = desc.extent;
		info.renderArea.offset = desc.offset;
		info.viewMask = desc.viewMask;
		info.layerCount = desc.layerCount;
		info.flags = desc.renderFlags;

		m_buffer.beginRendering(&info);
	}

	void VCmdBuffer::EndRendering() const
	{
		m_buffer.endRendering();
	}

	void VCmdBuffer::ImageBarrier(const ImageBarrierProps& desc) const
	{
		vk::ImageMemoryBarrier barrier = {};
		barrier.srcAccessMask = desc.srcAccessMask;
		barrier.dstAccessMask = desc.dstAccessMask;
		barrier.oldLayout = desc.oldLayout;
		barrier.newLayout = desc.newLayout;
		barrier.srcQueueFamilyIndex = desc.srcQueueFamilyIndex;
		barrier.dstQueueFamilyIndex = desc.dstQueueFamilyIndex;
		barrier.image = desc.image->GetVkImage();
		barrier.subresourceRange = vk::ImageSubresourceRange(desc.aspectMask, desc.baseMipLevel, desc.levelCount, desc.baseArrayLayer, desc.layerCount);

		m_buffer.pipelineBarrier(desc.srcPipeline, desc.dstPipeline, vk::DependencyFlags(), 0, nullptr, 0, nullptr, 1, &barrier);
	}

	void VCmdBuffer::Destroy()
	{
		if (m_buffer != VK_NULL_HANDLE)
		{
			m_rootDevice->GetVkDevice().freeCommandBuffers(m_props.pool->GetVkCmdPool(), m_buffer);
			m_buffer = VK_NULL_HANDLE;
		}
	}
}
