#pragma once

#include <Engine/Core/Core.h>

namespace MAGE
{
	class PlatformHardware final
	{
	public:
		static u16 GetCPUThreadCount();
		static u64 GetRAMSize();

	public:
		PlatformHardware() = delete;
		~PlatformHardware() = delete;
	};
}
