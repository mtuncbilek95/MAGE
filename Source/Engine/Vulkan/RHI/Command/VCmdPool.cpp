#include "VCmdPool.h"

#include "../Device/VDevice.h"
#include "../Queue/VQueue.h"
#include "../Command/VCmdBuffer.h"
#include "Engine/Vulkan/Core/VkAssert.h"

namespace MAGE
{
	VCmdPool::VCmdPool(const CmdPoolProps& desc, VDevice* device) : VkObject(device), m_props(desc)
	{
		vk::CommandPoolCreateInfo createInfo = {};
		createInfo.queueFamilyIndex = desc.queue->GetFamilyIndex();
		createInfo.flags = desc.createFlags;

		ErrorUtils::VkAssert(m_rootDevice->GetVkDevice().createCommandPool(&createInfo, nullptr, &m_pool), "VCmdPool");
	}

	VCmdPool::~VCmdPool()
	{
		Destroy();
	}

	Owned<VCmdBuffer> VCmdPool::CreateBuffer(vk::CommandBufferLevel level)
	{
		CmdBufferProps prop = {};
		prop.level = level;
		prop.pool = this;
		return MakeOwned<VCmdBuffer>(prop, m_rootDevice);
	}

	void VCmdPool::Destroy()
	{
		if (m_pool != VK_NULL_HANDLE)
		{
			m_rootDevice->GetVkDevice().destroyCommandPool(m_pool);
			m_pool = VK_NULL_HANDLE;
		}
	}
}
