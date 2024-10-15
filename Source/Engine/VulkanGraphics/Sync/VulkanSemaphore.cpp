#include "VulkanSemaphore.h"

#include "../Core/VkAssert.h"
#include "../Device/VulkanDevice.h"

namespace MAGE
{
	VulkanSemaphore::VulkanSemaphore(VulkanDevice* device) : m_deviceRef(device)
	{
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		ErrorUtils::VkAssert(vkCreateSemaphore(m_deviceRef->GetDevice(), &semaphoreInfo, nullptr, &m_semaphore));
	}

	VulkanSemaphore::~VulkanSemaphore()
	{
		if (m_semaphore != VK_NULL_HANDLE)
			vkDestroySemaphore(m_deviceRef->GetDevice(), m_semaphore, nullptr);
	}
}
