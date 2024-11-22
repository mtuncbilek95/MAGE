#include "VImageView.h"

#include "../Core/VkAssert.h"
#include "../Device/VDevice.h"

namespace MAGE
{
	VImageView::VImageView(const ImageViewProps& desc, VDevice* device) : VkObject(device), m_props(desc)
	{
		VkImageViewCreateInfo viewInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
		viewInfo.image = desc.ownerImage->GetVkImage();
		viewInfo.viewType = desc.viewType;
		viewInfo.format = desc.ownerImage->GetFormat();

		viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		viewInfo.subresourceRange.aspectMask = desc.aspectFlag;
		viewInfo.subresourceRange.baseMipLevel = desc.baseMipLevel;
		viewInfo.subresourceRange.levelCount = desc.ownerImage->GetMipLevels();
		viewInfo.subresourceRange.baseArrayLayer = desc.baseArrayLayer;
		viewInfo.subresourceRange.layerCount = desc.ownerImage->GetArrayLayers();

		ErrorUtils::VkAssert(vkCreateImageView(m_rootDevice->GetVkDevice(), &viewInfo, nullptr, &m_view), "VImageView");
	}

	VImageView::~VImageView()
	{
		Destroy();
	}

	void VImageView::Destroy()
	{
		if (m_view != VK_NULL_HANDLE)
		{
			vkDestroyImageView(m_rootDevice->GetVkDevice(), m_view, nullptr);
			m_view = VK_NULL_HANDLE;
		}
	}
}