#include "VSwapchain.h"

#include "../Device/VDevice.h"
#include "../Image/VImage.h"
#include "../Image/VImageView.h"

#include "Engine/Vulkan/Core/VkAssert.h"

namespace MAGE
{
	Swapchain::Swapchain(const SwapchainProps& desc, VDevice* device) : VkObject(device)
	{
	}

	Swapchain::~Swapchain()
	{
	}

	void Swapchain::Resize(const vk::Extent2D& newSize)
	{
	}

	void Swapchain::Destroy()
	{
	}
}
