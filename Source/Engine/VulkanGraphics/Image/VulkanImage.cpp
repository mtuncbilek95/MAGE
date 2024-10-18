#include "VulkanImage.h"

#include "../Core/VkAssert.h"
#include "../Device/VulkanDevice.h"

namespace MAGE
{
	VulkanImage::VulkanImage() : m_deviceRef(nullptr), m_image(VK_NULL_HANDLE), m_imageMemory(VK_NULL_HANDLE), m_props(), m_ownedImage(false)
	{
	}

	VulkanImage::VulkanImage(const ImageProps& desc, VulkanDevice* device) : m_deviceRef(device),
		m_image(VK_NULL_HANDLE), m_imageMemory(VK_NULL_HANDLE), m_props(desc), m_ownedImage(true)
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
		imageInfo.usage = desc.imageUsage;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;

		ErrorUtils::VkAssert(vkCreateImage(m_deviceRef->GetDevice(), &imageInfo, nullptr, &m_image), "VulkanImage");

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(m_deviceRef->GetDevice(), m_image, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = device->FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		ErrorUtils::VkAssert(vkAllocateMemory(m_deviceRef->GetDevice(), &allocInfo, nullptr, &m_imageMemory), "VulkanImage");
		ErrorUtils::VkAssert(vkBindImageMemory(m_deviceRef->GetDevice(), m_image, m_imageMemory, 0), "VulkanImage");
	}

	VulkanImage::VulkanImage(const ImageProps& desc, VkImage image, VulkanDevice* device) : m_deviceRef(device),
		m_image(image), m_imageMemory(VK_NULL_HANDLE), m_props(desc), m_ownedImage(false)
	{
	}

	VulkanImage::~VulkanImage()
	{
		if (m_ownedImage)
		{
			vkDestroyImage(m_deviceRef->GetDevice(), m_image, nullptr);
			vkFreeMemory(m_deviceRef->GetDevice(), m_imageMemory, nullptr);
		}
	}

	Owned<VulkanImageView> VulkanImage::CreateView(const ImageViewProps& desc)
	{
		return MakeOwned<VulkanImageView>(desc, this, m_deviceRef);
	}
}