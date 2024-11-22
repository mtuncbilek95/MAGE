#include "VDescPool.h"

#include "../Device/VDevice.h"

#include "Engine/Vulkan/Core/VkAssert.h"

namespace MAGE
{
	VDescPool::VDescPool(const DescPoolProps& desc, VDevice* device) : VkObject(device), m_props(desc)
	{
		VkDescriptorPoolCreateInfo poolInfo = { VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };
		poolInfo.maxSets = desc.maxSets;
		poolInfo.flags = desc.createFlag;
		poolInfo.poolSizeCount = static_cast<u32>(desc.poolSizes.size());
		poolInfo.pPoolSizes = desc.poolSizes.data();

		ErrorUtils::VkAssert(vkCreateDescriptorPool(m_rootDevice->GetVkDevice(), &poolInfo, nullptr, &m_pool), "VDescPool");
	}

	VDescPool::~VDescPool()
	{
		Destroy();
	}

	void VDescPool::Destroy()
	{
		if (m_pool != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorPool(m_rootDevice->GetVkDevice(), m_pool, nullptr);
			m_pool = VK_NULL_HANDLE;
		}
	}
}