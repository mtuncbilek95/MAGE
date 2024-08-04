#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Swapchain/Swapchain.h>

#include <vulkan/vulkan.h>

namespace MAGE
{
	class VDevice;
	class VSwapchain : public Swapchain
	{
	public:
		VSwapchain(const SwapchainDesc& desc, VDevice* pDevice);
		~VSwapchain() override;

		VkSwapchainKHR GetSwapchain() const { return mSwapchain; }
		VkSurfaceKHR GetSurfaceFormat() const { return mSurface; }

	protected:
		virtual u32 AcquireNextImageImpl(Fence* pFence, Semaphore* pSemaphore) override;
		virtual void PresentImpl(Semaphore* pSemaphore) override;
		virtual void ResizeSwapchainImpl(Vec2u newSize) override;

	private:
		VkSwapchainKHR mSwapchain;
		VkSurfaceKHR mSurface;
		VkDevice mDevice;
		VkPhysicalDevice mAdapter;
		VkInstance mInstance;
	};
}
