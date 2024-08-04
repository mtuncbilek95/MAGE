#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Monitor/Monitor.h>

namespace MAGE
{
	class PlatformMonitor
	{
	public:
		static DArray<Monitor> GetAllMonitors();
		static Monitor GetPrimaryMonitor();

	public:
		PlatformMonitor() = delete;
		~PlatformMonitor() = delete;
	};
}
