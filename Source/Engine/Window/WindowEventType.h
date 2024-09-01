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

		WET_FileDropped,

		WET_LeftMouseClicked,
		WET_LeftMouseReleased,
		WET_RightMouseClicked,
		WET_RightMouseReleased,

		WET_MouseMoved,
		WET_MouseScrolled,

		WET_KeyPressed,
		WET_KeyReleased,
		WET_KeyTyped,
	};
}
