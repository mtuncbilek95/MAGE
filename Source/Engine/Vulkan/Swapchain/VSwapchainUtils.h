#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Swapchain/PresentMode.h"

#include <vulkan/vulkan.h>

namespace MAGE
{
	namespace VkUtils
	{
		static VkPresentModeKHR GetVkPresentMode(PresentMode mode)
		{
			switch (mode)
			{
			case PresentMode::PM_Immediate:
				return VK_PRESENT_MODE_IMMEDIATE_KHR;
			case PresentMode::PM_VSyncImmediate:
				return VK_PRESENT_MODE_FIFO_RELAXED_KHR;
			case PresentMode::PM_VSyncQueued:
				return VK_PRESENT_MODE_MAILBOX_KHR;
			case PresentMode::PM_FullVSync:
				return VK_PRESENT_MODE_FIFO_KHR;
			default:
				return VK_PRESENT_MODE_FIFO_KHR;
			}
		}
	}
}
