#pragma once

#include <Engine/Core/Definitions.h>
#include <Engine/Core/StdFix.h>
#include <Engine/Core/Guid.h>

namespace MAGE
{
	class PlatformGuid final
	{
	public:
		static void GenerateGuid(Guid& guidOut);

	public:
		PlatformGuid() = delete;
		~PlatformGuid() = delete;
	};
}
