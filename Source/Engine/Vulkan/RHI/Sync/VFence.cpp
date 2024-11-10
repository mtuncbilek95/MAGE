#include "VFence.h"

#include "../Device/VDevice.h"
#include "Engine/Vulkan/Core/VkAssert.h"

namespace MAGE
{
	VFence::VFence(VDevice* device) : VkObject(device)
	{
		vk::FenceCreateInfo info = {};
		ErrorUtils::VkAssert(device->GetVkDevice().createFence(&info, nullptr, &m_fence), "VFence");
	}

	VFence::~VFence()
	{
		Destroy();
	}

	void VFence::Wait() const
	{
		ErrorUtils::VkAssert(m_rootDevice->GetVkDevice().waitForFences(1, &m_fence, false, u64_max), "VFence");
	}

	void VFence::Reset() const
	{
		ErrorUtils::VkAssert(m_rootDevice->GetVkDevice().resetFences(1, &m_fence), "VFence");
	}

	void VFence::Destroy()
	{
		if (m_fence != VK_NULL_HANDLE)
		{
			m_rootDevice->GetVkDevice().destroyFence(m_fence);
			m_fence = VK_NULL_HANDLE;
		}
	}
}
