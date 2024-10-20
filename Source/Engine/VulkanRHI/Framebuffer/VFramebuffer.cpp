#include "VFramebuffer.h"

#include "../Core/VAssert.h"
#include "../Core/VLoadFuncs.h"
#include "../Device/VDevice.h"
#include "../RenderPass/VRenderPass.h"
#include "../Image/VImageView.h"

namespace MAGE
{
	VFramebuffer::VFramebuffer(const FramebufferProps& desc, VDevice* device) : VObject(device), 
		m_props(desc), m_framebuffer(VK_NULL_HANDLE)
	{
		VkImageView view = desc.imageView->GetView();

		VkFramebufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		createInfo.renderPass = desc.renderPass->GetRenderPass();
		createInfo.attachmentCount = 1;
		createInfo.pAttachments = &view;
		createInfo.width = desc.imageView->GetImageSize().x;
		createInfo.height = desc.imageView->GetImageSize().y;
		createInfo.layers = 1;

		ErrorUtils::VkAssert(vkCreateFramebuffer(m_rootDevice->GetDevice(), &createInfo, nullptr, &m_framebuffer), "VFramebuffer");
	}

	VFramebuffer::~VFramebuffer()
	{
		Destroy();
	}

	Math::Vec2u VFramebuffer::GetRenderArea() const
	{
		return m_props.imageView->GetImageSize();
	}

	void VFramebuffer::Destroy()
	{
		if (m_framebuffer != VK_NULL_HANDLE)
		{
			vkDestroyFramebuffer(m_rootDevice->GetDevice(), m_framebuffer, nullptr);
			m_framebuffer = VK_NULL_HANDLE;
		}
	}
}
