#include "VulkanDescPool.h"

#include "../Core/VkAssert.h"
#include "../Core/VkFunctions.h"
#include "../Device/VulkanDevice.h"

namespace MAGE
{
	VulkanDescPool::VulkanDescPool() : m_deviceRef(nullptr), m_props(), m_pool(VK_NULL_HANDLE)
	{
	}

	VulkanDescPool::VulkanDescPool(const DescPoolProps& desc, VulkanDevice* device) : m_deviceRef(device),
		m_props(desc), m_pool(VK_NULL_HANDLE)
	{
		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.maxSets = desc.maxSets;
		poolInfo.flags = desc.flags;
		poolInfo.poolSizeCount = static_cast<u32>(desc.poolSizes.size());
		poolInfo.pPoolSizes = desc.poolSizes.data();

		ErrorUtils::VkAssert(vkCreateDescriptorPool(m_deviceRef->GetDevice(), &poolInfo, nullptr, &m_pool), "VulkanDescPool");
	}

	VulkanDescPool::~VulkanDescPool()
	{
		if (m_pool != VK_NULL_HANDLE)
			vkDestroyDescriptorPool(m_deviceRef->GetDevice(), m_pool, nullptr);
	}
}
