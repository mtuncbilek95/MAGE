#include "VFence.h"

#include "../Core/VAssert.h"
#include "../Core/VLoadFuncs.h"
#include "../Device/VDevice.h"

namespace MAGE
{
	VFence::VFence(bool signaled, VDevice* device) : VObject(device), m_fence(VK_NULL_HANDLE)
	{
		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;
		
		ErrorUtils::VkAssert(vkCreateFence(m_rootDevice->GetDevice(), &fenceInfo, nullptr, &m_fence), "VFence");
	}

	VFence::~VFence()
	{
		Destroy();
	}

	void VFence::Destroy()
	{
		if (m_fence != VK_NULL_HANDLE)
		{
			vkDestroyFence(m_rootDevice->GetDevice(), m_fence, nullptr);
			m_fence = VK_NULL_HANDLE;
		}
	}
}
