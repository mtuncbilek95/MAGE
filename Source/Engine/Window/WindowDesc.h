#pragma once

#include "Engine/Core/Definitions.h"
#include "Engine/Core/StdFix.h"
#include "Engine/Window/WindowMode.h"

namespace MAGE
{
	struct WindowDesc
	{
		Math::Vec2u WindowSize;
		Math::Vec2i WindowPosition;
		String WindowTitle;
		WindowMode WindowMode;
	};
}
