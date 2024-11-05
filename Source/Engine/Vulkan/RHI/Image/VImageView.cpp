#include "VImageView.h"

#include "../Device/VDevice.h"

#include "Engine/Vulkan/Core/VkAssert.h"

namespace MAGE
{
	VImageView::VImageView(const ImageViewProps& desc, VDevice* device) : VkObject(device), m_props(desc)
	{
		vk::ImageViewCreateInfo viewInfo = {};
		viewInfo.image = desc.ownerImage->GetVkImage();
		viewInfo.viewType = desc.viewType;
		viewInfo.format = desc.ownerImage->GetFormat();
		viewInfo.components = vk::ComponentSwizzle();

		viewInfo.subresourceRange.aspectMask = desc.aspectFlag;
		viewInfo.subresourceRange.baseMipLevel = desc.baseMipLevel;
		viewInfo.subresourceRange.levelCount = desc.ownerImage->GetMipLevels();
		viewInfo.subresourceRange.baseArrayLayer = desc.baseArrayLayer;
		viewInfo.subresourceRange.layerCount = desc.ownerImage->GetArrayLayers();

		ErrorUtils::VkAssert(m_rootDevice->GetVkDevice().createImageView(&viewInfo, nullptr, &m_view), "VImageView");
	}

	VImageView::~VImageView()
	{
		Destroy();
	}

	void VImageView::Destroy()
	{
		if (m_view != VK_NULL_HANDLE)
		{
			m_rootDevice->GetVkDevice().destroyImageView(m_view);
			m_view = VK_NULL_HANDLE;
		}
	}
}
