#include "VulkanCmdBuffer.h"

#include "../Core/VkAssert.h"
#include "../Device/VulkanDevice.h"
#include "../Command/VulkanCmdPool.h"
#include "../RenderPass/VulkanRenderPass.h"
#include "../Image/VulkanImageView.h"

namespace MAGE
{
	VulkanCmdBuffer::VulkanCmdBuffer(const CmdBufferProps& desc, VulkanDevice* device) : m_deviceRef(device), m_props(desc),
		m_cmdPool(desc.m_pool->GetCmdPool())
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
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = useFlag;

		ErrorUtils::VkAssert(vkBeginCommandBuffer(m_cmdBuffer, &beginInfo), "VulkanCmdBuffer");
	}

	void VulkanCmdBuffer::EndRecording() const
	{
		ErrorUtils::VkAssert(vkEndCommandBuffer(m_cmdBuffer), "VulkanCmdBuffer");
	}

	void VulkanCmdBuffer::BeginRenderPass(VulkanRenderPass* renderPass, Math::Vec2u renderArea, u32 imageIndex) const
	{
		VkRenderingAttachmentInfo colorInfo = {};
		colorInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
		colorInfo.imageView = renderPass->GetColorAttachment(imageIndex)->GetImageView();
		colorInfo.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		colorInfo.resolveMode = VK_RESOLVE_MODE_NONE;
		colorInfo.resolveImageView = VK_NULL_HANDLE;
		colorInfo.resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorInfo.clearValue.color = { renderPass->GetClearColor().x, renderPass->GetClearColor().y, renderPass->GetClearColor().z, renderPass->GetClearColor().w };

		VkRenderingAttachmentInfo depthInfo = {};
		depthInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
		depthInfo.imageView = renderPass->GetDepthAttachment() ? renderPass->GetDepthAttachment()->GetImageView() : VK_NULL_HANDLE;
		depthInfo.imageLayout = renderPass->GetDepthAttachment() ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_UNDEFINED;
		depthInfo.resolveMode = VK_RESOLVE_MODE_NONE;
		depthInfo.resolveImageView = VK_NULL_HANDLE;
		depthInfo.resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		depthInfo.clearValue.depthStencil = { renderPass->GetClearDepth(), 0 };

		VkRenderingInfo renderInfo = {};
		renderInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO_KHR;
		renderInfo.colorAttachmentCount = 1;
		renderInfo.pColorAttachments = &colorInfo;
		renderInfo.pDepthAttachment = &depthInfo;
		renderInfo.layerCount = 1;
		renderInfo.renderArea.extent = { renderArea.x, renderArea.y };
		renderInfo.renderArea.offset = { 0, 0 };

		vkCmdBeginRendering(m_cmdBuffer, &renderInfo);
	}

	void VulkanCmdBuffer::EndRenderPass() const
	{
		vkCmdEndRendering(m_cmdBuffer);
	}
}
