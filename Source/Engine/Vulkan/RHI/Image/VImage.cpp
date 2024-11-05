#include "VImage.h"

#include "../Device/VDevice.h"
#include "../Image/VImageView.h"

#include "Engine/Vulkan/Core/VkAssert.h"

namespace MAGE
{
	namespace ViewHelper
	{
		static vk::ImageViewType CastType(vk::ImageType type, u32 layerCount)
		{
			if (type == vk::ImageType::e2D) {
				if (layerCount == 6) {
					return vk::ImageViewType::eCube;
				}
				else if (layerCount > 6 && layerCount % 6 == 0) {
					return vk::ImageViewType::eCubeArray;
				}
			}

			switch (type) {
			case vk::ImageType::e1D:
				return layerCount > 1 ? vk::ImageViewType::e1DArray : vk::ImageViewType::e1D;
			case vk::ImageType::e2D:
				return layerCount > 1 ? vk::ImageViewType::e2DArray : vk::ImageViewType::e2D;
			case vk::ImageType::e3D:
				return layerCount > 1 ? vk::ImageViewType::e2DArray : vk::ImageViewType::e3D;
			default:
				spdlog::error("You're using wrong vk::ImageType or vk::ImageViewType");
				break;
			}

			return vk::ImageViewType::e2D;
		}
	}

	VImage::VImage(const ImageProps& desc, VDevice* device) : VkObject(device),
		m_props(desc), m_owned(true)
	{
		vk::ImageCreateInfo imageInfo = {};
		imageInfo.extent = desc.imageSize;
		imageInfo.mipLevels = desc.mipLevels;
		imageInfo.arrayLayers = desc.arrayLayers;
		imageInfo.imageType = desc.imageType;
		imageInfo.format = desc.imageFormat;
		imageInfo.usage = desc.imageUsage;

		ErrorUtils::VkAssert(m_rootDevice->GetVkDevice().createImage(&imageInfo, nullptr, &m_image), "VImage");

		// TODO: Allocate memory
	}

	VImage::VImage(const ImageProps& desc, vk::Image img, VDevice* device) : VkObject(device),
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
			m_rootDevice->GetVkDevice().destroyImage(m_image);
			m_image = VK_NULL_HANDLE;
		}

		// TODO: Free memory
	}
}
