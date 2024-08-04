#pragma once

#include <Engine/Core/Definitions.h>
#include <Engine/Core/StdFix.h>

namespace MAGE
{
	class PlatformError final
	{
	public:
		static String GetPlatformError();

	public:
		PlatformError() = delete;
		~PlatformError() = delete;
	};
}
