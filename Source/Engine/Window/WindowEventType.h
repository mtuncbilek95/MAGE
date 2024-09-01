#pragma once

#include "Engine/Core/Definitions.h"
#include "Engine/Core/StdFix.h"

namespace MAGE
{
	enum class WindowEventType
	{
		WET_None,
		WET_WindowClosed,
		WET_WindowMoved,
		WET_WindowResized,
		WET_WindowFocused,
		WET_WindowLostFocus,
		WET_WindowMinimized,
		WET_WindowMaximized,
		WET_FileDropped
	};
}
