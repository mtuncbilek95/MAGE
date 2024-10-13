#include "VulkanRenderPass.h"

#include "../Device/VulkanDevice.h"
#include "Engine/VulkanGraphics/Image/VulkanImageView.h"

namespace MAGE
{
	VulkanRenderPass::VulkanRenderPass(const RenderPassProps& desc, VulkanDevice* device)
		: m_deviceRef(device), m_props(desc), m_depthAttachment(nullptr)
	{
	}

	VulkanRenderPass::~VulkanRenderPass()
	{
		m_colorAttachments.clear();
		m_colorAttachments.shrink_to_fit();
		m_depthAttachment = nullptr;
	}

	void VulkanRenderPass::AddAttachment(VulkanImageView* imageView)
	{
		if (imageView->GetAspectFlags() & VK_IMAGE_ASPECT_COLOR_BIT)
			m_colorAttachments.push_back(imageView);
		else if (imageView->GetAspectFlags() & VK_IMAGE_ASPECT_DEPTH_BIT)
			m_depthAttachment = imageView;
	}

	void VulkanRenderPass::RemoveAttachment(VulkanImageView* imageView)
	{
		if (imageView->GetAspectFlags() & VK_IMAGE_ASPECT_COLOR_BIT)
		{
			auto it = std::find(m_colorAttachments.begin(), m_colorAttachments.end(), imageView);
			if (it != m_colorAttachments.end())
				m_colorAttachments.erase(it);
		}
		else if (imageView->GetAspectFlags() & VK_IMAGE_ASPECT_DEPTH_BIT)
		{
			if (m_depthAttachment == imageView)
				m_depthAttachment = nullptr;
		}
	}
}
