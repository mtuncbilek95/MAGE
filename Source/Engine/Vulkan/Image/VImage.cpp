#include "VImage.h"

#include "../Core/VkAssert.h"
#include "../Device/VDevice.h"
#include "../Image/VImageView.h"

namespace MAGE
{
	namespace ViewHelper
	{
		static VkImageViewType CastType(VkImageType type, u32 layerCount)
		{
			if (type == VK_IMAGE_TYPE_2D) {
				if (layerCount == 6) {
					return VK_IMAGE_VIEW_TYPE_CUBE;
				}
				else if (layerCount > 6 && layerCount % 6 == 0) {
					return VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
				}
			}

			switch (type) {
			case VK_IMAGE_TYPE_1D:
				return layerCount > 1 ? VK_IMAGE_VIEW_TYPE_1D_ARRAY : VK_IMAGE_VIEW_TYPE_1D;
			case VK_IMAGE_TYPE_2D:
				return layerCount > 1 ? VK_IMAGE_VIEW_TYPE_2D_ARRAY : VK_IMAGE_VIEW_TYPE_2D;
			case VK_IMAGE_TYPE_3D:
				return layerCount > 1 ? VK_IMAGE_VIEW_TYPE_2D_ARRAY : VK_IMAGE_VIEW_TYPE_3D;
			default:
				spdlog::error("You're using wrong vk::ImageType or vk::ImageViewType");
				break;
			}

			return VK_IMAGE_VIEW_TYPE_2D;
		}
	}

	VImage::VImage(const ImageProps& desc, VDevice* device) : VkObject(device),
		m_props(desc), m_owned(true)
	{
		VkImageCreateInfo imageInfo = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
		imageInfo.extent = desc.imageSize;
		imageInfo.mipLevels = desc.mipLevels;
		imageInfo.arrayLayers = desc.arrayLayers;
		imageInfo.imageType = desc.imageType;
		imageInfo.format = desc.imageFormat;
		imageInfo.usage = desc.imageUsage;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;

		ErrorUtils::VkAssert(vkCreateImage(m_rootDevice->GetVkDevice(), &imageInfo, nullptr, &m_image), "VImage");

		// TODO: Allocate memory
	}

	VImage::VImage(const ImageProps& desc, VkImage img, VDevice* device) : VkObject(device),
		m_props(desc), m_image(img), m_owned(false)
	{
	}

	VImage::~VImage()
	{
		Destroy();
	}

	Owned<VImageView> VImage::CreateView(const ViewProps& desc)
	{
		ImageViewProps viewProp(this,
			ViewHelper::CastType(m_props.imageType, m_props.arrayLayers),
			desc.aspectFlag, desc.baseMipLevel, desc.baseArrayLayer);

		return MakeOwned<VImageView>(viewProp, m_rootDevice);
	}

	void VImage::Destroy()
	{
		if (m_owned && m_image != VK_NULL_HANDLE)
		{
			vkDestroyImage(m_rootDevice->GetVkDevice(), m_image, nullptr);
			m_image = VK_NULL_HANDLE;
		}

		// TODO: Free memory
	}
}
