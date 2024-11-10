#include "VDescPool.h"

#include "../Device/VDevice.h"

#include "Engine/Vulkan/Core/VkAssert.h"

namespace MAGE
{
	VDescPool::VDescPool(const DescPoolProps& desc, VDevice* device) : VkObject(device), m_props(desc)
	{
		vk::DescriptorPoolCreateInfo poolInfo = {};
		poolInfo.maxSets = desc.maxSets;
		poolInfo.flags = desc.createFlag;
		poolInfo.poolSizeCount = static_cast<u32>(desc.poolSizes.size());
		poolInfo.pPoolSizes = desc.poolSizes.data();

		ErrorUtils::VkAssert(m_rootDevice->GetVkDevice().createDescriptorPool(&poolInfo, nullptr, &m_pool), "VDescPool");
	}

	VDescPool::~VDescPool()
	{
		Destroy();
	}

	void VDescPool::Destroy()
	{
		if (m_pool != VK_NULL_HANDLE)
		{
			m_rootDevice->GetVkDevice().destroyDescriptorPool(m_pool);
			m_pool = VK_NULL_HANDLE;
		}
	}
}
