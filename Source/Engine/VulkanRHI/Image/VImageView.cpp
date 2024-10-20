#include "VImageView.h"

#include "../Core/VAssert.h"
#include "../Core/VLoadFuncs.h"
#include "../Device/VDevice.h"
#include "../Image/VImage.h"

namespace MAGE
{
	VImageView::VImageView(const ImageViewProps& desc, VImage* image, VDevice* device) : VObject(device), 
		m_props(desc), m_imageRef(image), m_view(VK_NULL_HANDLE)
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

		ErrorUtils::VkAssert(vkCreateImageView(m_rootDevice->GetDevice(), &viewInfo, nullptr, &m_view), "VulkanImageView");
	}

	VImageView::~VImageView()
	{
		Destroy();
	}

	Math::Vec3u VImageView::GetImageSize() const
	{
		return m_imageRef->GetImageSize();
	}

	u32 VImageView::GetMipLevels() const
	{
		return m_imageRef->GetMipLevels();
	}

	u32 VImageView::GetArrayLayers() const
	{
		return m_imageRef->GetArrayLayers();
	}

	VkImageType VImageView::GetImageType() const
	{
		return m_imageRef->GetImageType();
	}

	VkFormat VImageView::GetFormat() const
	{
		return m_imageRef->GetFormat();
	}

	VkImageUsageFlags VImageView::GetImageUsage() const
	{
		return m_imageRef->GetImageUsage();
	}

	void VImageView::Destroy()
	{
		if (m_view != VK_NULL_HANDLE)
		{
			vkDestroyImageView(m_rootDevice->GetDevice(), m_view, nullptr);
			m_view = VK_NULL_HANDLE;
		}
	}
}

