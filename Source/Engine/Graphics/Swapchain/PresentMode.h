#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	enum class ENGINE_API PresentMode : u8
	{
		PM_Immediate = 0, // VK_IMMEDIATE
		PM_VSyncImmediate = 1, // VK_FIFO_RELAXED
		PM_VSyncQueued = 2, // VK_MAILBOX
		PM_FullVSync = 3// VK_FIFO
	};
}