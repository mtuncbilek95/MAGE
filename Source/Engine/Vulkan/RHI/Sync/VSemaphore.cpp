#include "VSemaphore.h"

#include "../Device/VDevice.h"
#include "Engine/Vulkan/Core/VkAssert.h"

namespace MAGE
{
	VSemaphore::VSemaphore(VDevice* device) : VkObject(device)
	{
		vk::SemaphoreCreateInfo info = {};
		ErrorUtils::VkAssert(device->GetVkDevice().createSemaphore(&info, nullptr, &m_semaphore), "VFence");
	}

	VSemaphore::~VSemaphore()
	{
		Destroy();
	}

	void VSemaphore::Destroy()
	{
		if (m_semaphore != VK_NULL_HANDLE)
		{
			m_rootDevice->GetVkDevice().destroySemaphore(m_semaphore);
			m_semaphore = VK_NULL_HANDLE;
		}
	}
}
