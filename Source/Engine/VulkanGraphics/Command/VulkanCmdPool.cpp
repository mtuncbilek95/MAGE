#include "VulkanCmdPool.h"

#include "../Core/VkAssert.h"
#include "../Device/VulkanDevice.h"
#include "../Queue/VulkanQueue.h"
#include "../Command/VulkanCmdBuffer.h"

namespace MAGE
{
	VulkanCmdPool::VulkanCmdPool(const CmdPoolProps& desc, VulkanDevice* device) : m_device(device->GetDevice()), m_deviceRef(device)
	{
		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = desc.m_queue->GetFamilyIndex();
		poolInfo.flags = desc.m_flags;

		ErrorUtils::VkAssert(vkCreateCommandPool(m_device, &poolInfo, nullptr, &m_cmdPool), "VulkanCmdPool");
	}

	VulkanCmdPool::~VulkanCmdPool()
	{
		vkDestroyCommandPool(m_device, m_cmdPool, nullptr);
	}

	Shared<VulkanCmdBuffer> VulkanCmdPool::CreateCmdBuffer(VkCommandBufferLevel cmdLevel)
	{
		CmdBufferProps props = {};
		props.m_cmdLevel = cmdLevel;
		props.m_pool = this;

		return MakeShared<VulkanCmdBuffer>(props, m_deviceRef);
	}
}
