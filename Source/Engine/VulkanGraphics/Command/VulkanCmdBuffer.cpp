#include "VulkanCmdBuffer.h"

#include "../Core/VkAssert.h"
#include "../Device/VulkanDevice.h"
#include "../Command/VulkanCmdPool.h"
#include "../RenderPass/VulkanRenderPass.h"
#include "../Image/VulkanImageView.h"

namespace MAGE
{
	VulkanCmdBuffer::VulkanCmdBuffer(const CmdBufferProps& desc, VulkanDevice* device) : m_deviceRef(device), m_props(desc)
	{
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = desc.m_pool->GetCmdPool();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		ErrorUtils::VkAssert(vkAllocateCommandBuffers(m_deviceRef->GetDevice(), &allocInfo, &m_cmdBuffer));
	}

	VulkanCmdBuffer::~VulkanCmdBuffer()
	{
		vkFreeCommandBuffers(m_deviceRef->GetDevice(), m_props.m_pool->GetCmdPool(), 1, &m_cmdBuffer);
	}

	void VulkanCmdBuffer::BeginRecording(VkCommandBufferUsageFlags useFlag)
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		beginInfo.pInheritanceInfo = nullptr;

		vkBeginCommandBuffer(m_cmdBuffer, &beginInfo);
	}

	void VulkanCmdBuffer::EndRecording()
	{
		vkEndCommandBuffer(m_cmdBuffer);
	}

	void VulkanCmdBuffer::BeginRenderPass(VulkanImageView* view, Math::Vec2u renderArea)
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

	void VulkanCmdBuffer::EndRenderPass()
	{
		vkCmdEndRendering(m_cmdBuffer);
	}
}
