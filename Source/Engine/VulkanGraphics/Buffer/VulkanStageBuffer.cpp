#include "VulkanStageBuffer.h"

#include "../Core/VkAssert.h"
#include "../Device/VulkanDevice.h"

namespace MAGE
{
	VulkanStageBuffer::VulkanStageBuffer(const StageBufferProps& desc, VulkanDevice* device) : m_deviceRef(device), m_buffer(VK_NULL_HANDLE), 
		m_memory(VK_NULL_HANDLE), m_totalSize(0), m_offset(0), m_props(desc)
	{
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = desc.sizeInBytes;
		bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

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
	}

	VulkanStageBuffer::~VulkanStageBuffer()
	{
		if (m_buffer != VK_NULL_HANDLE)
			vkDestroyBuffer(m_deviceRef->GetDevice(), m_buffer, nullptr);

		if (m_memory != VK_NULL_HANDLE)
			vkFreeMemory(m_deviceRef->GetDevice(), m_memory, nullptr);
	}

	void VulkanStageBuffer::MapMemory(const VulkanMapProps& desc, RawBuffer buffer)
	{
		void* data;
		ErrorUtils::VkAssert(vkMapMemory(m_deviceRef->GetDevice(), m_memory, desc.offset, m_totalSize, desc.flags, &data), "VulkanDescBuffer");
		memcpy(data, buffer.Data(), buffer.Size());
	}
}
