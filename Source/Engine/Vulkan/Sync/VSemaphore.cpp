#include "VSemaphore.h"

#include "../Core/VkAssert.h"
#include "../Device/VDevice.h"

namespace MAGE
{
	VSemaphore::VSemaphore(VDevice* device) : VkObject(device)
	{
		VkSemaphoreCreateInfo semaphoreInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };
		ErrorUtils::VkAssert(vkCreateSemaphore(m_rootDevice->GetVkDevice(), &semaphoreInfo, nullptr, &m_semaphore), "VSemaphore");
	}

	VSemaphore::~VSemaphore()
	{
		Destroy();
	}

	void VSemaphore::Destroy()
	{
		if (m_semaphore != VK_NULL_HANDLE)
		{
			vkDestroySemaphore(m_rootDevice->GetVkDevice(), m_semaphore, nullptr);
			m_semaphore = VK_NULL_HANDLE;
		}
	}
}
