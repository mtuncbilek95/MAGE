#pragma once

#include "Engine/Core/Definitions.h"
#include "Engine/Core/StdFix.h"

namespace MAGE
{
	enum class WindowEventType
	{
		None,
		WindowClosed,
		WindowMoved,
		WindowResized,
		WindowFocused,
		WindowLostFocus,
		WindowMinimized,
		WindowMaximized,
		FileDropped
	};
}
