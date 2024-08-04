#pragma once

#include <Engine/Core/Definitions.h>
#include <Engine/Core/StdFix.h>
#include <Engine/Time/Time.h>

namespace MAGE
{
	class Win32Time final
	{
	public:
		static Time CurrentDateTime();
	};
}
