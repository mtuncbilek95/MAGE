#include "VulkanCmdBuffer.h"

#include "../Core/VkAssert.h"
#include "../Device/VulkanDevice.h"
#include "../Command/VulkanCmdPool.h"

namespace MAGE
{
	VulkanCmdBuffer::VulkanCmdBuffer(const CmdBufferProps& desc, VulkanDevice* device) : m_device(device->GetDevice()), m_props(desc), 
		m_cmdPool(desc.m_pool->GetCmdPool())
	{
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = desc.m_pool->GetCmdPool();
		allocInfo.level = desc.m_cmdLevel;
		allocInfo.commandBufferCount = 1;

		ErrorUtils::VkAssert(vkAllocateCommandBuffers(m_device, &allocInfo, &m_cmdBuffer), "VulkanCmdBuffer");
	}

	VulkanCmdBuffer::~VulkanCmdBuffer()
	{
		vkFreeCommandBuffers(m_device, m_props.m_pool->GetCmdPool(), 1, &m_cmdBuffer);
	}
}
