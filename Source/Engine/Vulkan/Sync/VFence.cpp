#include "VFence.h"

#include <Engine/Vulkan/Device/VDevice.h>

namespace MAGE
{
	VFence::VFence(b8 bSignalled, VDevice* pDevice) : Fence(bSignalled, pDevice), mDevice(pDevice->GetVkDevice())
	{
		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = bSignalled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

		CORE_ASSERT(vkCreateFence(mDevice, &fenceInfo, nullptr, &mFence) == VK_SUCCESS, "VFence", "Failed to create fence!");
	}
}
