#include "VulkanCmdPool.h"

#include "../Core/VkAssert.h"
#include "../Device/VulkanDevice.h"
#include "../Queue/VulkanQueue.h"
#include "../Command/VulkanCmdBuffer.h"

namespace MAGE
{
	VulkanCmdPool::VulkanCmdPool(const CmdPoolProps& desc, VulkanDevice* device) : m_deviceRef(device)
	{
		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = desc.queue->GetFamilyIndex();
		poolInfo.flags = desc.flags;

		ErrorUtils::VkAssert(vkCreateCommandPool(m_deviceRef->GetDevice(), &poolInfo, nullptr, &m_cmdPool), "VulkanCmdPool");
	}

	VulkanCmdPool::~VulkanCmdPool()
	{
		vkDestroyCommandPool(m_deviceRef->GetDevice(), m_cmdPool, nullptr);
	}

	Shared<VulkanCmdBuffer> VulkanCmdPool::CreateCmdBuffer(VkCommandBufferLevel cmdLevel)
	{
		CmdBufferProps props = {};
		props.m_cmdLevel = cmdLevel;
		props.m_pool = shared_from_this().get();

		return MakeShared<VulkanCmdBuffer>(props, m_deviceRef);
	}
}
