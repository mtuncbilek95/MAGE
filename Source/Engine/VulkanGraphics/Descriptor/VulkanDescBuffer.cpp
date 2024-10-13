#include "VulkanDescBuffer.h"
#include "VulkanDescLayout.h"

#include "../Core/VkAssert.h"
#include "../Device/VulkanDevice.h"

namespace MAGE
{
	u64 AlignDescBuffer(u64 value, u64 alignment)
	{
		return (value + alignment - 1) & ~(alignment - 1);
	}

	VulkanDescBuffer::VulkanDescBuffer(VulkanDescLayout* layout, VulkanDevice* device) : m_deviceRef(device), 
		m_layoutRef(layout), m_totalSize(0), m_offset(0)
	{
		// Why the fuck do I need this? There is a function for it already.
		PFN_vkGetDescriptorSetLayoutSizeEXT vkGetDescriptorSetLayoutSizeEXT =
			(PFN_vkGetDescriptorSetLayoutSizeEXT)vkGetDeviceProcAddr(m_deviceRef->GetDevice(), "vkGetDescriptorSetLayoutSizeEXT");
		PFN_vkGetDescriptorSetLayoutBindingOffsetEXT vkGetDescriptorSetLayoutBindingOffsetEXT =
			(PFN_vkGetDescriptorSetLayoutBindingOffsetEXT)vkGetDeviceProcAddr(m_deviceRef->GetDevice(), "vkGetDescriptorSetLayoutBindingOffsetEXT");

		// Get descriptor buffer properties
		VkPhysicalDeviceDescriptorBufferPropertiesEXT descriptorBufferProperties = {};
		descriptorBufferProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_PROPERTIES_EXT;
		VkPhysicalDeviceProperties2 deviceProperties2 = {};
		deviceProperties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
		deviceProperties2.pNext = &descriptorBufferProperties;
		vkGetPhysicalDeviceProperties2(m_deviceRef->GetAdapter(), &deviceProperties2);

		// Get layout size
		VkDeviceSize memorySize;
		vkGetDescriptorSetLayoutSizeEXT(m_deviceRef->GetDevice(), m_layoutRef->GetLayout(), &memorySize);

		// Align memory size
		VkDeviceSize offsetSize;
		memorySize = AlignDescBuffer(memorySize, descriptorBufferProperties.descriptorBufferOffsetAlignment);
		vkGetDescriptorSetLayoutBindingOffsetEXT(m_deviceRef->GetDevice(), m_layoutRef->GetLayout(), 0u, &offsetSize);

		VkBufferUsageFlags2CreateInfoKHR usageFlags = {};
		usageFlags.sType = VK_STRUCTURE_TYPE_BUFFER_USAGE_FLAGS_2_CREATE_INFO_KHR;
		usageFlags.usage = VK_BUFFER_USAGE_2_RESOURCE_DESCRIPTOR_BUFFER_BIT_EXT;

		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = memorySize * 4;
		bufferInfo.usage = VK_BUFFER_USAGE_RESOURCE_DESCRIPTOR_BUFFER_BIT_EXT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		bufferInfo.pNext = &usageFlags;

		ErrorUtils::VkAssert(vkCreateBuffer(m_deviceRef->GetDevice(), &bufferInfo, nullptr, &m_buffer), "VulkanDescBuffer");

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(m_deviceRef->GetDevice(), m_buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = device->FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

		ErrorUtils::VkAssert(vkAllocateMemory(m_deviceRef->GetDevice(), &allocInfo, nullptr, &m_memory), "VulkanDescBuffer");
		ErrorUtils::VkAssert(vkBindBufferMemory(m_deviceRef->GetDevice(), m_buffer, m_memory, 0), "VulkanDescBuffer");

		m_totalSize = bufferInfo.size;
		m_offset = offsetSize;
	}

	VulkanDescBuffer::~VulkanDescBuffer()
	{
		vkDestroyBuffer(m_deviceRef->GetDevice(), m_buffer, nullptr);
		vkFreeMemory(m_deviceRef->GetDevice(), m_memory, nullptr);
	}
}
