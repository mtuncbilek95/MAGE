#include "VBuffer.h"

#include "../Device/VDevice.h"
#include "../Memory/MemoryAllocator.h"
#include "Engine/Vulkan/Core/VkAssert.h"

namespace MAGE
{
	VBuffer::VBuffer(const BufferProps& desc, VDevice* device) : VkObject(device), m_props(desc), m_memoryOffset(0), m_memory(nullptr)
	{
		vk::BufferCreateInfo bufferInfo = vk::BufferCreateInfo();
		bufferInfo.size = desc.sizeInBytes;
		bufferInfo.usage = vk::BufferUsageFlagBits::eShaderDeviceAddress | desc.usageFlags;

		ErrorUtils::VkAssert(m_rootDevice->GetVkDevice().createBuffer(&bufferInfo, nullptr, &m_buffer), "VBuffer");
	}

	VBuffer::~VBuffer()
	{
		Destroy();
	}

	vk::DeviceAddress VBuffer::GetVkAddress() const
	{
		vk::BufferDeviceAddressInfo addressInfo = {};
		addressInfo.buffer = m_buffer;
		m_memory->m_address = m_rootDevice->GetVkDevice().getBufferAddress(&addressInfo);
		return m_memory->m_address;
	}

	usize VBuffer::GetRequestedSize() const
	{
		vk::MemoryRequirements memReq = vk::MemoryRequirements();
		m_rootDevice->GetVkDevice().getBufferMemoryRequirements(m_buffer, &memReq);
		return memReq.size + memReq.alignment;
	}

	void VBuffer::BindMemory(VMemory* memory)
	{
		vk::MemoryRequirements memReq = vk::MemoryRequirements();
		m_rootDevice->GetVkDevice().getBufferMemoryRequirements(m_buffer, &memReq);

		u64 offset = memory->Allocate(memReq.size + memReq.alignment);
		m_memoryOffset = offset + (offset % memReq.alignment == 0 ? 0 : (memReq.alignment - (offset % memReq.alignment)));

		m_rootDevice->GetVkDevice().bindBufferMemory(m_buffer, memory->m_memory, m_memoryOffset);

		m_memory = memory;
	}

	void VBuffer::Update(RawBuffer buffer) const
	{
		memcpy(m_memory->m_mappedData + m_memoryOffset, buffer.Data(), buffer.Size());
	}

	void VBuffer::Destroy()
	{
		if (m_buffer != VK_NULL_HANDLE)
		{
			m_rootDevice->GetVkDevice().destroyBuffer(m_buffer);
			m_buffer = VK_NULL_HANDLE;

			m_memory->Free(0, m_memoryOffset);
		}
	}
}
