#include "VImage.h"

#include "../Core/VAssert.h"
#include "../Core/VLoadFuncs.h"
#include "../Device/VDevice.h"

namespace MAGE
{
	VImage::VImage(const ImageProps& desc, VDevice* device) : VObject(device), 
		m_props(desc), m_owned(true), m_image(VK_NULL_HANDLE), m_memory(VK_NULL_HANDLE)
	{
		VkImageCreateInfo imageInfo = {};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = desc.imageType;
		imageInfo.extent = { desc.imageSize.x, desc.imageSize.y, desc.imageSize.z };
		imageInfo.mipLevels = desc.mipLevels;
		imageInfo.arrayLayers = desc.arrayLayers;
		imageInfo.format = desc.format;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | desc.imageUsage;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;

		ErrorUtils::VkAssert(vkCreateImage(m_rootDevice->GetDevice(), &imageInfo, nullptr, &m_image), "VulkanImage");

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(m_rootDevice->GetDevice(), m_image, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = device->FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		ErrorUtils::VkAssert(vkAllocateMemory(m_rootDevice->GetDevice(), &allocInfo, nullptr, &m_memory), "VulkanImage");
		ErrorUtils::VkAssert(vkBindImageMemory(m_rootDevice->GetDevice(), m_image, m_memory, 0), "VulkanImage");
	}

	VImage::VImage(const ImageProps& desc, VkImage image, VDevice* device) : VObject(device),
		m_props(desc), m_owned(false), m_image(image), m_memory(VK_NULL_HANDLE)
	{
	}

	VImage::~VImage()
	{
		Destroy();
	}

	Owned<VImageView> VImage::CreateView(const ImageViewProps& desc)
	{
		return MakeOwned<VImageView>(desc, this, m_rootDevice);
	}

	void VImage::Destroy()
	{
		if (m_owned && m_image != VK_NULL_HANDLE)
		{
			vkDestroyImage(m_rootDevice->GetDevice(), m_image, nullptr);
			m_image = VK_NULL_HANDLE;
		}
	}
}
