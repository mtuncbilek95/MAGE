#include "VCombinedBuffer.h"

#include "../Core/VAssert.h"
#include "../Core/VLoadFuncs.h"
#include "../Device/VDevice.h"

namespace MAGE
{
	VCombinedBuffer::VCombinedBuffer(const CombinedBufferProps& desc, VDevice* device) : VObject(device), m_props(desc)
	{
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = desc.sizeInBytes;
		bufferInfo.usage = desc.usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		ErrorUtils::VkAssert(vkCreateBuffer(m_rootDevice->GetDevice(), &bufferInfo, nullptr, &m_buffer), "VCombinedBuffer");

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(m_rootDevice->GetDevice(), m_buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = device->FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		VkMemoryAllocateFlagsInfo allocationFlags = {};
		allocationFlags.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO;
		allocationFlags.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT;

		allocInfo.pNext = &allocationFlags;

		ErrorUtils::VkAssert(vkAllocateMemory(m_rootDevice->GetDevice(), &allocInfo, nullptr, &m_memory), "VCombinedBuffer");
		ErrorUtils::VkAssert(vkBindBufferMemory(m_rootDevice->GetDevice(), m_buffer, m_memory, 0), "VCombinedBuffer");

		m_totalSize = desc.sizeInBytes;
	}

	VCombinedBuffer::~VCombinedBuffer()
	{
		Destroy();
	}

	VkDeviceAddress VCombinedBuffer::GetDeviceAddress() const
	{
		VkBufferDeviceAddressInfo deviceAddressInfo{};
		deviceAddressInfo.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
		deviceAddressInfo.buffer = m_buffer;
		VkDeviceAddress address = vkGetBufferDeviceAddress(m_rootDevice->GetDevice(), &deviceAddressInfo);
		return address;
	}

	void VCombinedBuffer::MapMemory(RawBuffer buffer)
	{
		ErrorUtils::VkAssert(vkMapMemory(m_rootDevice->GetDevice(), m_memory, 0, m_totalSize, 0, reinterpret_cast<void**>(&m_data)), "VCombinedBuffer");
		memcpy(m_data, buffer.Data(), buffer.Size());
	}

	void VCombinedBuffer::Destroy()
	{
		if (m_data)
		{
			vkUnmapMemory(m_rootDevice->GetDevice(), m_memory);
			m_data = nullptr;
		}

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
