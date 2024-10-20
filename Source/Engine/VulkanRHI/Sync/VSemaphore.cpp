#include "VSemaphore.h"

#include "../Core/VAssert.h"
#include "../Core/VLoadFuncs.h"
#include "../Device/VDevice.h"

namespace MAGE
{
	VSemaphore::VSemaphore(VDevice* device) : VObject(device), m_semaphore(VK_NULL_HANDLE)
	{
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		ErrorUtils::VkAssert(vkCreateSemaphore(m_rootDevice->GetDevice(), &semaphoreInfo, nullptr, &m_semaphore), "VSemaphore");
	}

	VSemaphore::~VSemaphore()
	{
		Destroy();
	}

	void VSemaphore::Destroy()
	{
		if (m_semaphore != VK_NULL_HANDLE)
		{
			vkDestroySemaphore(m_rootDevice->GetDevice(), m_semaphore, nullptr);
			m_semaphore = VK_NULL_HANDLE;
		}
	}
}
