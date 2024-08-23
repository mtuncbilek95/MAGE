#pragma once

#include <Engine/Core/Definitions.h>
#include <Engine/Core/StdFix.h>
#include <Engine/Window/WindowMode.h>

namespace MAGE
{
	struct WindowDesc
	{
		Vec2u WindowSize;
		Vec2i WindowPosition;
		String WindowTitle;
		WindowMode CurrentWindowMode;
	};
}
