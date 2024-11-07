#include "VDescBuffer.h"

#include "../Device/VDevice.h"
#include "../Memory/MemoryAllocator.h"
#include "../Descriptor/VDescLayout.h"
#include "Engine/Vulkan/Core/VkAssert.h"

namespace MAGE
{
	VDescBuffer::VDescBuffer(const DescBufferProps& desc, VDevice* device) : VkObject(device), m_props(desc), m_memory(nullptr), m_memoryOffset(0), m_layoutSize(0)
	{
		vk::PhysicalDeviceDescriptorBufferPropertiesEXT bufferProps = desc.layout->GetBufferProps();
		
		m_rootDevice->GetVkDevice().getDescriptorSetLayoutSizeEXT(desc.layout->GetVkLayout(), &m_layoutSize);

		m_layoutSize = AlignMemory(m_layoutSize, bufferProps.descriptorBufferOffsetAlignment);
		m_rootDevice->GetVkDevice().getDescriptorSetLayoutBindingOffsetEXT(desc.layout->GetVkLayout(), 0u, &m_bufferOffset);


		vk::BufferUsageFlags2CreateInfoKHR usageFlags = {};
		usageFlags.usage = vk::BufferUsageFlagBits2KHR::eShaderDeviceAddress | vk::BufferUsageFlags2KHR(static_cast<VkFlags>(desc.usageFlags));

		vk::BufferCreateInfo createInfo = {};
		createInfo.size = m_layoutSize * desc.resourceCount;
		createInfo.usage = vk::BufferUsageFlagBits::eShaderDeviceAddress | desc.usageFlags;
		createInfo.pNext = &usageFlags;

		ErrorUtils::VkAssert(m_rootDevice->GetVkDevice().createBuffer(&createInfo, nullptr, &m_buffer), "VDescBuffer");

		vk::MemoryRequirements memReq = vk::MemoryRequirements();
		m_rootDevice->GetVkDevice().getBufferMemoryRequirements(m_buffer, &memReq);

		m_memory = device->GetAllocator()->GetAvailableMemory(AllocProps(createInfo.size, vk::MemoryPropertyFlagBits::eDeviceLocal | vk::MemoryPropertyFlagBits::eHostVisible));

		u64 offset = m_memory->Allocate(memReq.size + memReq.alignment);
		m_memoryOffset = offset + (offset % memReq.alignment == 0 ? 0 : (memReq.alignment - (offset % memReq.alignment)));

		m_rootDevice->GetVkDevice().bindBufferMemory(m_buffer, m_memory->m_memory, m_memoryOffset);
	}

	VDescBuffer::~VDescBuffer()
	{
		Destroy();
	}

	vk::DeviceAddress VDescBuffer::GetVkAddress() const
	{
		vk::BufferDeviceAddressInfo addressInfo = {};
		addressInfo.buffer = m_buffer;
		m_memory->m_address = m_rootDevice->GetVkDevice().getBufferAddress(&addressInfo);
		return m_memory->m_address;
	}

	void VDescBuffer::Update(RawBuffer buffer) const
	{
		memcpy(m_memory->m_mappedData + m_memoryOffset, buffer.Data(), buffer.Size());
	}

	void VDescBuffer::SetupData(const Vector<VBuffer*>& buffers)
	{
		// TODO: Implement this carefully.
	}

	void VDescBuffer::Destroy()
	{
		if (m_buffer != VK_NULL_HANDLE)
		{
			m_rootDevice->GetVkDevice().destroyBuffer(m_buffer);
			m_buffer = VK_NULL_HANDLE;

			m_memory->Free(0, m_memoryOffset);
		}
	}

	u64 VDescBuffer::AlignMemory(u64 value, u64 alignment) const
	{
		return (value + alignment - 1) & ~(alignment - 1);
	}
}
