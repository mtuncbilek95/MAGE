#include "VDescBuffer.h"

#include "../Core/VAssert.h"
#include "../Core/VLoadFuncs.h"
#include "../Device/VDevice.h"
#include "../Descriptor/VDescLayout.h"

namespace MAGE
{
	u64 AlignDescBuffer(u64 value, u64 alignment)
	{
		return (value + alignment - 1) & ~(alignment - 1);
	}

	VDescBuffer::VDescBuffer(const DescBufferProps& desc, VDevice* device) : VObject(device),
		m_props(desc), m_memory(VK_NULL_HANDLE), m_buffer(VK_NULL_HANDLE)
	{
		// Get descriptor buffer properties
		VkPhysicalDeviceDescriptorBufferPropertiesEXT descriptorBufferProperties = {};
		descriptorBufferProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_BUFFER_PROPERTIES_EXT;
		VkPhysicalDeviceProperties2 deviceProperties2 = {};
		deviceProperties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
		deviceProperties2.pNext = &descriptorBufferProperties;
		vkGetPhysicalDeviceProperties2(m_rootDevice->GetAdapter(), &deviceProperties2);

		// Get layout size
		VkDeviceSize memorySize;
		GetDescriptorSetLayoutSizeEXT(m_rootDevice->GetDevice(), desc.layout->GetLayout(), &memorySize);

		// Align memory size
		VkDeviceSize offsetSize;
		memorySize = AlignDescBuffer(memorySize, descriptorBufferProperties.descriptorBufferOffsetAlignment);
		GetDescriptorSetLayoutBindingOffsetEXT(m_rootDevice->GetDevice(), desc.layout->GetLayout(), 0u, &offsetSize);

		VkBufferUsageFlags2CreateInfoKHR usageFlags = {};
		usageFlags.sType = VK_STRUCTURE_TYPE_BUFFER_USAGE_FLAGS_2_CREATE_INFO_KHR;
		usageFlags.usage = VK_BUFFER_USAGE_2_RESOURCE_DESCRIPTOR_BUFFER_BIT_EXT;

		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = memorySize * desc.insideBufferCount;
		bufferInfo.usage = VK_BUFFER_USAGE_RESOURCE_DESCRIPTOR_BUFFER_BIT_EXT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | desc.extraFlag;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		bufferInfo.pNext = &usageFlags;

		ErrorUtils::VkAssert(vkCreateBuffer(m_rootDevice->GetDevice(), &bufferInfo, nullptr, &m_buffer), "VulkanDescBuffer");

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(m_rootDevice->GetDevice(), m_buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = device->FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		ErrorUtils::VkAssert(vkAllocateMemory(m_rootDevice->GetDevice(), &allocInfo, nullptr, &m_memory), "VulkanDescBuffer");
		ErrorUtils::VkAssert(vkBindBufferMemory(m_rootDevice->GetDevice(), m_buffer, m_memory, 0), "VulkanDescBuffer");

		m_totalSize = bufferInfo.size;
		m_offset = offsetSize;
	}

	VDescBuffer::~VDescBuffer()
	{
		Destroy();
	}

	inline VkDeviceAddress VDescBuffer::GetAddress()
	{
		VkBufferDeviceAddressInfo addressInfo = {};
		addressInfo.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
		addressInfo.buffer = m_buffer;
		m_address = vkGetBufferDeviceAddress(m_rootDevice->GetDevice(), &addressInfo);
		return m_address;
	}

	void VDescBuffer::MapMemory(RawBuffer buffer, u64 offset)
	{
		void* data;
		ErrorUtils::VkAssert(vkMapMemory(m_rootDevice->GetDevice(), m_memory, offset, m_totalSize, 0, &data), "VulkanDescBuffer");
		memcpy(data, buffer.Data(), buffer.Size());
		vkUnmapMemory(m_rootDevice->GetDevice(), m_memory);
	}

	void VDescBuffer::Destroy()
	{
		if (m_memory != VK_NULL_HANDLE)
		{
			vkFreeMemory(m_rootDevice->GetDevice(), m_memory, nullptr);
			m_memory = VK_NULL_HANDLE;
		}

		if (m_buffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(m_rootDevice->GetDevice(), m_buffer, nullptr);
			m_buffer = VK_NULL_HANDLE;
		}
	}
}
