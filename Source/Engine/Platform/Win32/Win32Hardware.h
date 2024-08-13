#pragma once

#include <Engine/Core/Core.h>

namespace MAGE
{
	class Win32Hardware
	{
	public:
		static u16 GetCPUThreadCount();
		static u64 GetRAMSize();

	public:
		Win32Hardware() = delete;
		~Win32Hardware() = delete;
	};
}
