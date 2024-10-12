#include "VulkanImage.h"

#include "../Core/VkAssert.h"
#include "../Device/VulkanDevice.h"

namespace MAGE
{
	VulkanImage::VulkanImage(const ImageProps& desc, VulkanDevice* device) : m_imageSize(desc.imageSize), 
		m_mipLevels(desc.mipLevels), m_arrayLayers(desc.arrayLayers), m_imageType(desc.imageType), 
		m_format(desc.format), m_imageUsage(desc.imageUsage), m_device(device->GetDevice()), m_physicalDevice(device->GetAdapter()),
		m_deviceRef(device)
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

		ErrorUtils::VkAssert(vkCreateImage(m_device, &imageInfo, nullptr, &m_image), "VulkanImage");

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(m_device, m_image, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = device->FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		ErrorUtils::VkAssert(vkAllocateMemory(m_device, &allocInfo, nullptr, &m_imageMemory), "VulkanImage");
		ErrorUtils::VkAssert(vkBindImageMemory(m_device, m_image, m_imageMemory, 0), "VulkanImage");
	}

	VulkanImage::~VulkanImage()
	{
		vkDestroyImage(m_device, m_image, nullptr);
		vkFreeMemory(m_device, m_imageMemory, nullptr);
	}

	Owned<VulkanImageView> VulkanImage::CreateView(const ImageViewProps& desc)
	{
		return MakeOwned<VulkanImageView>(desc, this, m_deviceRef);
	}
}