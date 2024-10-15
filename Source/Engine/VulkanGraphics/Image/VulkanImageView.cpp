#include "VulkanImageView.h"
#include "VulkanImage.h"

#include "../Core/VkAssert.h"
#include "../Device/VulkanDevice.h"

namespace MAGE
{
	VulkanImageView::VulkanImageView(const ImageViewProps& desc, VulkanImage* image, VulkanDevice* device) : m_props(desc), m_imageRef(image), m_deviceRef(device)
	{
		VkImageViewCreateInfo viewInfo = {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = m_imageRef->GetImage();
		viewInfo.viewType = desc.viewType;
		viewInfo.format = image->GetFormat();

		viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		viewInfo.subresourceRange.aspectMask = desc.aspectFlags;
		viewInfo.subresourceRange.baseMipLevel = desc.baseMipLevel;
		viewInfo.subresourceRange.levelCount = image->GetMipLevels();
		viewInfo.subresourceRange.baseArrayLayer = desc.baseArrayLayer;
		viewInfo.subresourceRange.layerCount = image->GetArrayLayers();

		ErrorUtils::VkAssert(vkCreateImageView(m_deviceRef->GetDevice(), &viewInfo, nullptr, &m_imageView));
	}

	VulkanImageView::~VulkanImageView()
	{
		if (m_imageView != VK_NULL_HANDLE)
			vkDestroyImageView(m_deviceRef->GetDevice(), m_imageView, nullptr);
	}
}