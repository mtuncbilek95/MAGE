#include "VDescPool.h"

#include "../Core/VAssert.h"
#include "../Core/VLoadFuncs.h"
#include "../Device/VDevice.h"

namespace MAGE
{
	VDescPool::VDescPool(const DescPoolProps& desc, VDevice* device) : VObject(device), 
		m_props(desc), m_pool(VK_NULL_HANDLE)
	{
		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.maxSets = desc.maxSets;
		poolInfo.flags = desc.flags;
		poolInfo.poolSizeCount = static_cast<u32>(desc.poolSizes.size());
		poolInfo.pPoolSizes = desc.poolSizes.data();

		ErrorUtils::VkAssert(vkCreateDescriptorPool(m_rootDevice->GetDevice(), &poolInfo, nullptr, &m_pool), "VDescPool");
	}

	VDescPool::~VDescPool()
	{
		Destroy();
	}

	void VDescPool::Destroy()
	{
		if (m_pool != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorPool(m_rootDevice->GetDevice(), m_pool, nullptr);
			m_pool = VK_NULL_HANDLE;
		}
	}
}


