#include "VBuffer.h"

#include "../Core/VkAssert.h"
#include "../Device/VDevice.h"

namespace MAGE
{
	VBuffer::VBuffer(const BufferProps& desc, VDevice* device) : VkObject(device), m_props(desc), m_memoryOffset(0), m_memory(nullptr)
	{
		VkBufferCreateInfo bufferInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
		bufferInfo.size = desc.sizeInBytes;
		bufferInfo.usage = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | desc.usageFlags;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		ErrorUtils::VkAssert(vkCreateBuffer(m_rootDevice->GetVkDevice(), &bufferInfo, nullptr, &m_buffer), "VBuffer");
	}

	VBuffer::~VBuffer()
	{
		Destroy();
	}

	VkDeviceAddress VBuffer::GetVkAddress() const
	{
		VkBufferDeviceAddressInfo addressInfo = {};
		addressInfo.buffer = m_buffer;
		m_memory->m_address = vkGetBufferDeviceAddress(m_rootDevice->GetVkDevice(), &addressInfo);
		return m_memory->m_address;
	}

	usize VBuffer::GetRequestedSize() const
	{
		VkMemoryRequirements memReq = {};
		vkGetBufferMemoryRequirements(m_rootDevice->GetVkDevice(), m_buffer, &memReq);
		return memReq.size + memReq.alignment;
	}

	void VBuffer::BindMemory(VMemory* memory)
	{
		VkMemoryRequirements memReq = {};
		vkGetBufferMemoryRequirements(m_rootDevice->GetVkDevice(), m_buffer, &memReq);

		u64 offset = memory->Allocate(memReq.size + memReq.alignment);
		m_memoryOffset = offset + (offset % memReq.alignment == 0 ? 0 : (memReq.alignment - (offset % memReq.alignment)));

		vkBindBufferMemory(m_rootDevice->GetVkDevice(), m_buffer, memory->m_memory, m_memoryOffset);

		m_memory = memory;
	}

	void VBuffer::Update(RawBuffer buffer, usize offset) const
	{
		memcpy(m_memory->m_mappedData + m_memoryOffset + offset, static_cast<u8*>(buffer.Data()), buffer.Size());
	}

	void VBuffer::Destroy()
	{
		if (m_buffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(m_rootDevice->GetVkDevice(), m_buffer, nullptr);
			m_buffer = VK_NULL_HANDLE;

			m_memory->Free(0, m_memoryOffset);
		}
	}
}