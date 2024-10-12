#include "VulkanImageView.h"
#include "VulkanImage.h"

#include "../Core/VkAssert.h"
#include "../Device/VulkanDevice.h"

namespace MAGE
{
	VulkanImageView::VulkanImageView(const ImageViewProps& desc, VulkanImage* image, VulkanDevice* device) : m_image(image->GetImage()), m_device(device->GetDevice()),
		m_imageRef(image), m_deviceRef(device), m_viewType(desc.viewType), m_aspectFlags(desc.aspectFlags), m_baseMipLevel(desc.baseMipLevel), m_baseArrayLayer(desc.baseArrayLayer)
	{
		VkImageViewCreateInfo viewInfo = {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = m_image;
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

		ErrorUtils::VkAssert(vkCreateImageView(device->GetDevice(), &viewInfo, nullptr, &m_imageView), "VulkanImage");
	}

	VulkanImageView::~VulkanImageView()
	{
		vkDestroyImageView(m_device, m_imageView, nullptr);
	}
}