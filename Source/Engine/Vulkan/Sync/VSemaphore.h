#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Sync/Semaphore.h>

#include <vulkan/vulkan.h>

namespace MAGE
{
	class VDevice;
	class VSemaphore : public Semaphore
	{
	public:
		VSemaphore(VDevice* pDevice);
		~VSemaphore() override = default;

		VkSemaphore GetVkSemaphore() const { return mSemaphore; }

	private:
		VkSemaphore mSemaphore;
		VkDevice mDevice;
	};
}
