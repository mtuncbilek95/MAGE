#include "VulkanFence.h"

#include "../Core/VkAssert.h"
#include "../Device/VulkanDevice.h"

namespace MAGE
{
	VulkanFence::VulkanFence(bool signaled, VulkanDevice* device) : m_deviceRef(device)
	{
		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

		ErrorUtils::VkAssert(vkCreateFence(m_deviceRef->GetDevice(), &fenceInfo, nullptr, &m_fence));
	}

	VulkanFence::~VulkanFence()
	{
		if (m_fence != VK_NULL_HANDLE)
			vkDestroyFence(m_deviceRef->GetDevice(), m_fence, nullptr);
	}
}
