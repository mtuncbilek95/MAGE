#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Monitor/Monitor.h"

namespace MAGE
{
	class ENGINE_API PlatformMonitor
	{
	public:
		static Vector<Monitor> GetAllMonitors();
		static Monitor GetPrimaryMonitor();

	public:
		PlatformMonitor() = delete;
		~PlatformMonitor() = delete;
	};
}
