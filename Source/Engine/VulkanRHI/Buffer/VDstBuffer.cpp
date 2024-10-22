#include "VDstBuffer.h"

#include "../Core/VAssert.h"
#include "../Core/VLoadFuncs.h"
#include "../Device/VDevice.h"

namespace MAGE
{
	VDstBuffer::VDstBuffer(const DstBufferProps& desc, VDevice* device) : VObject(device), m_props(desc)
	{
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = desc.sizeInBytes;
		bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | desc.usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		ErrorUtils::VkAssert(vkCreateBuffer(m_rootDevice->GetDevice(), &bufferInfo, nullptr, &m_buffer), "VulkanDescBuffer");

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(m_rootDevice->GetDevice(), m_buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = device->FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		ErrorUtils::VkAssert(vkAllocateMemory(m_rootDevice->GetDevice(), &allocInfo, nullptr, &m_memory), "VulkanDescBuffer");
		ErrorUtils::VkAssert(vkBindBufferMemory(m_rootDevice->GetDevice(), m_buffer, m_memory, 0), "VulkanDescBuffer");

		m_totalSize = desc.sizeInBytes;
	}

	VDstBuffer::~VDstBuffer()
	{
		Destroy();
	}

	void VDstBuffer::MapMemory(RawBuffer buffer)
	{
		void* data;
		ErrorUtils::VkAssert(vkMapMemory(m_rootDevice->GetDevice(), m_memory, 0, m_totalSize, 0, &data), "VulkanDescBuffer");
		memcpy(data, buffer.Data(), buffer.Size());
		vkUnmapMemory(m_rootDevice->GetDevice(), m_memory);
	}

	void VDstBuffer::Destroy()
	{
		if (m_buffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(m_rootDevice->GetDevice(), m_buffer, nullptr);
			m_buffer = VK_NULL_HANDLE;
		}

		if (m_memory != VK_NULL_HANDLE)
		{
			vkFreeMemory(m_rootDevice->GetDevice(), m_memory, nullptr);
			m_memory = VK_NULL_HANDLE;
		}
	}
}
