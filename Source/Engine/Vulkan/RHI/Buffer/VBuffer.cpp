#include "VBuffer.h"

#include "../Device/VDevice.h"
#include "Engine/Vulkan/Core/VkAssert.h"

namespace MAGE
{
	VBuffer::VBuffer(const BufferProps& desc, VDevice* device) : VkObject(device), m_props(desc)
	{
		vk::BufferCreateInfo bufferInfo = vk::BufferCreateInfo();
		bufferInfo.size = 0;
		bufferInfo.flags;
		
		ErrorUtils::VkAssert(m_rootDevice->GetVkDevice().createBuffer(&bufferInfo, nullptr, &m_buffer), "VBuffer");

		// TODO: Allocate memory

	}

	VBuffer::~VBuffer()
	{
		Destroy();
	}

	void VBuffer::Map() const
	{
	}

	void VBuffer::Update(RawBuffer buffer)
	{
	}

	void VBuffer::Unmap() const
	{
		// if mem is mapped do the unmap
		vk::DeviceMemory mem;
		m_rootDevice->GetVkDevice().unmapMemory(mem);
	}

	void VBuffer::Destroy()
	{
		Unmap();

		if (m_buffer != VK_NULL_HANDLE)
		{
			m_rootDevice->GetVkDevice().destroyBuffer(m_buffer);
			m_buffer = VK_NULL_HANDLE;
		}

		// TODO: Free memory
	}
}
