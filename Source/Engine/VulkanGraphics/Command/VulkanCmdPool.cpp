#include "VulkanCmdPool.h"

#include "../Core/VkAssert.h"
#include "../Device/VulkanDevice.h"
#include "../Queue/VulkanQueue.h"
#include "../Command/VulkanCmdBuffer.h"

namespace MAGE
{
	VulkanCmdPool::VulkanCmdPool(const CmdPoolProps& desc, VulkanDevice* device) : m_deviceRef(device), m_props(desc)
	{
		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = desc.queue->GetFamilyIndex();
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		ErrorUtils::VkAssert(vkCreateCommandPool(m_deviceRef->GetDevice(), &poolInfo, nullptr, &m_cmdPool));
	}

	VulkanCmdPool::~VulkanCmdPool()
	{
		vkDestroyCommandPool(m_deviceRef->GetDevice(), m_cmdPool, nullptr);
	}

	Owned<VulkanCmdBuffer> VulkanCmdPool::CreateCmdBuffer(VkCommandBufferLevel cmdLevel)
	{
		CmdBufferProps props = {};
		props.m_cmdLevel = cmdLevel;
		props.m_pool = this;

		return MakeOwned<VulkanCmdBuffer>(props, m_deviceRef);
	}
}
