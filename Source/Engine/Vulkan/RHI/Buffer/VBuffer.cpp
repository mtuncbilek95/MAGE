#include "VBuffer.h"

#include "../Device/VDevice.h"
#include "../Memory/MemoryAllocator.h"
#include "Engine/Vulkan/Core/VkAssert.h"

namespace MAGE
{
	VBuffer::VBuffer(const BufferProps& desc, VDevice* device) : VkObject(device), m_props(desc)
	{
		vk::BufferCreateInfo bufferInfo = vk::BufferCreateInfo();
		bufferInfo.size = desc.sizeInBytes;
		bufferInfo.usage = desc.usageFlags;

		ErrorUtils::VkAssert(m_rootDevice->GetVkDevice().createBuffer(&bufferInfo, nullptr, &m_buffer), "VBuffer");

		vk::MemoryRequirements memReq = vk::MemoryRequirements();
		m_rootDevice->GetVkDevice().getBufferMemoryRequirements(m_buffer, &memReq);

		u64 offset = desc.memory->Allocate(memReq.size + memReq.alignment);
		m_memoryOffset = offset + (offset % memReq.alignment == 0 ? 0 : (memReq.alignment - (offset % memReq.alignment)));

		m_rootDevice->GetVkDevice().bindBufferMemory(m_buffer, m_props.memory->m_memory, m_memoryOffset);
	}

	VBuffer::~VBuffer()
	{
		Destroy();
	}

	void VBuffer::Map()
	{
		//m_rootDevice->GetVkDevice().mapMemory(m_props.memory->m_memory, m_actualOffset, m_props.sizeInBytes, {}, m_mappedData);
	}

	void VBuffer::Update(RawBuffer buffer)
	{
		memcpy(m_mappedData, buffer.Data(), buffer.Size());
	}

	void VBuffer::Unmap() const
	{
		m_rootDevice->GetVkDevice().unmapMemory(m_props.memory->m_memory);
	}

	void VBuffer::Destroy()
	{
		if (m_buffer != VK_NULL_HANDLE)
		{
			m_rootDevice->GetVkDevice().destroyBuffer(m_buffer);
			m_buffer = VK_NULL_HANDLE;

			m_props.memory->Free(0, m_memoryOffset);
		}
	}
}
