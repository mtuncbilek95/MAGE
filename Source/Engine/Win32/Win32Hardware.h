#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	struct Win32Hardware final
	{
		static const String GetCPUName();
		static const String GetGPUName();
		static u64 GetHostMemorySize();
		static u64 GetDeviceMemorySize();
		static u32 GetCPUCores();
		static u32 GetCPUThreads();
	};
}