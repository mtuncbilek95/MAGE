#include "VCmdPool.h"

#include "../Core/VAssert.h"
#include "../Core/VLoadFuncs.h"
#include "../Device/VDevice.h"
#include "../Queue/VQueue.h"
#include "../Command/VCmdBuffer.h"

namespace MAGE
{
	VCmdPool::VCmdPool(const CmdPoolProps& desc, VDevice* device) : VObject(device), 
		m_props(desc), m_cmdPool(VK_NULL_HANDLE)
	{
		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = desc.queue->GetFamilyIndex();
		poolInfo.flags = desc.flags;

		ErrorUtils::VkAssert(vkCreateCommandPool(m_rootDevice->GetDevice(), &poolInfo, nullptr, &m_cmdPool), "VCmdPool");
	}

	VCmdPool::~VCmdPool()
	{
		Destroy();
	}

	Owned<VCmdBuffer> VCmdPool::CreateCmdBuffer(VkCommandBufferLevel cmdLevel)
	{
		CmdBufferProps prop =
		{
			.m_cmdLevel = cmdLevel,
			.m_pool = this
		};
		return MakeOwned<VCmdBuffer>(prop, m_rootDevice);
	}

	void VCmdPool::Destroy()
	{
		if (m_cmdPool != VK_NULL_HANDLE)
		{
			vkDestroyCommandPool(m_rootDevice->GetDevice(), m_cmdPool, nullptr);
			m_cmdPool = VK_NULL_HANDLE;
		}
	}
}
