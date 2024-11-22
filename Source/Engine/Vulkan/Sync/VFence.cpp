#include "VFence.h"

#include "../Core/VkAssert.h"
#include "../Device/VDevice.h"

namespace MAGE
{
	VFence::VFence(b8 signalled, VDevice* device) : VkObject(device)
	{
		VkFenceCreateInfo fenceInfo = { VK_STRUCTURE_TYPE_FENCE_CREATE_INFO };
		fenceInfo.flags = signalled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;
		ErrorUtils::VkAssert(vkCreateFence(m_rootDevice->GetVkDevice(), &fenceInfo, nullptr, &m_fence), "VFence");
	}

	VFence::~VFence()
	{
		Destroy();
	}

	void VFence::Wait() const
	{
		vkWaitForFences(m_rootDevice->GetVkDevice(), 1, &m_fence, VK_TRUE, u64_max);
	}

	void VFence::Reset() const
	{
		vkResetFences(m_rootDevice->GetVkDevice(), 1, &m_fence);
	}

	void VFence::Destroy()
	{
		if (m_fence != VK_NULL_HANDLE)
		{
			vkDestroyFence(m_rootDevice->GetVkDevice(), m_fence, nullptr);
			m_fence = VK_NULL_HANDLE;
		}
	}
}
