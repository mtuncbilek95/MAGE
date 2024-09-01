#include "VSemaphore.h"

#include "Engine/Vulkan/Device/VDevice.h"
#include "Engine/Platform/PlatformErrorMessage.h"

namespace MAGE
{
	VSemaphore::VSemaphore(VDevice* pDevice) : Semaphore(pDevice), mDevice(pDevice->GetVkDevice())
	{
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		Helpers::MageAssert(vkCreateSemaphore(mDevice, &semaphoreInfo, nullptr, &mSemaphore) == VK_SUCCESS, "VSemaphore", "Failed to create semaphore!");
	}
}
