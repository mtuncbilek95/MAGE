#include "VulkanCmdBuffer.h"

#include "../Core/VkAssert.h"
#include "../Device/VulkanDevice.h"
#include "../Command/VulkanCmdPool.h"
#include "../RenderPass/VulkanRenderPass.h"
#include "../Image/VulkanImage.h"
#include "../Image/VulkanImageView.h"

namespace MAGE
{
	VulkanCmdBuffer::VulkanCmdBuffer(const CmdBufferProps& desc, VulkanDevice* device) : m_deviceRef(device), m_props(desc)
	{
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = desc.m_pool->GetCmdPool();
		allocInfo.level = desc.m_cmdLevel;
		allocInfo.commandBufferCount = 1;

		ErrorUtils::VkAssert(vkAllocateCommandBuffers(m_deviceRef->GetDevice(), &allocInfo, &m_cmdBuffer), "VulkanCmdBuffer");
	}

	VulkanCmdBuffer::~VulkanCmdBuffer()
	{
		vkFreeCommandBuffers(m_deviceRef->GetDevice(), m_props.m_pool->GetCmdPool(), 1, &m_cmdBuffer);
	}

	void VulkanCmdBuffer::BeginRecording(VkCommandBufferUsageFlags useFlag) const
	{
		ErrorUtils::VkAssert(vkResetCommandBuffer(m_cmdBuffer, 0), "VulkanCmdBuffer");

		VkCommandBufferInheritanceInfo inheritanceInfo = {};
		inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
		inheritanceInfo.renderPass = VK_NULL_HANDLE;
		inheritanceInfo.subpass = 0;
		inheritanceInfo.framebuffer = VK_NULL_HANDLE;
		inheritanceInfo.occlusionQueryEnable = VK_FALSE;
		inheritanceInfo.queryFlags = 0;
		inheritanceInfo.pipelineStatistics = 0;

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = useFlag;
		beginInfo.pInheritanceInfo = m_props.m_cmdLevel & VK_COMMAND_BUFFER_LEVEL_SECONDARY ? &inheritanceInfo : nullptr;

		ErrorUtils::VkAssert(vkBeginCommandBuffer(m_cmdBuffer, &beginInfo), "VulkanCmdBuffer");
	}

	void VulkanCmdBuffer::EndRecording() const
	{
		ErrorUtils::VkAssert(vkEndCommandBuffer(m_cmdBuffer), "VulkanCmdBuffer");
	}

	void VulkanCmdBuffer::BeginRenderPass(VulkanImageView* view, Math::Vec2u renderArea) const
	{
		VkRenderingAttachmentInfo colorInfo = {};
		colorInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
		colorInfo.imageView = view->GetImageView();
		colorInfo.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		colorInfo.resolveImageView = VK_NULL_HANDLE;
		colorInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorInfo.clearValue.color = { 0.1f, 0.2f, 0.3f, 1.f };

		VkRenderingInfo renderInfo = {};
		renderInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR;
		renderInfo.colorAttachmentCount = 1;
		renderInfo.pColorAttachments = &colorInfo;
		renderInfo.pDepthAttachment = nullptr;
		renderInfo.layerCount = 1;
		renderInfo.viewMask = 0;
		renderInfo.renderArea.extent = { renderArea.x, renderArea.y };
		renderInfo.renderArea.offset = { 0, 0 };

		vkCmdBeginRendering(m_cmdBuffer, &renderInfo);
	}

	void VulkanCmdBuffer::EndRenderPass() const
	{
		vkCmdEndRendering(m_cmdBuffer);
	}

	void VulkanCmdBuffer::SetPipelineImageBarrier(const VulkanImageBarrier& barrier, VkPipelineStageFlagBits srcFlag, VkPipelineStageFlagBits dstFlag) const
	{
		VkImageMemoryBarrier barrierInfo = {};
		barrierInfo.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrierInfo.oldLayout = barrier.oldLayout;
		barrierInfo.newLayout = barrier.newLayout;
		barrierInfo.srcAccessMask = barrier.srcAccess;
		barrierInfo.dstAccessMask = barrier.dstAccess;
		barrierInfo.srcQueueFamilyIndex = barrier.srcFamilyIndex;
		barrierInfo.dstQueueFamilyIndex = barrier.dstFamilyIndex;
		barrierInfo.image = barrier.image->GetImage();
		barrierInfo.subresourceRange.aspectMask = barrier.aspect;
		barrierInfo.subresourceRange.baseMipLevel = barrier.baseMipLevel;
		barrierInfo.subresourceRange.levelCount = barrier.image->GetMipLevels();
		barrierInfo.subresourceRange.baseArrayLayer = barrier.baseLayer;
		barrierInfo.subresourceRange.layerCount = barrier.image->GetArrayLayers();

		vkCmdPipelineBarrier(m_cmdBuffer, srcFlag, dstFlag, 0, 0, nullptr, 0, nullptr, 1, &barrierInfo);
	}
}
