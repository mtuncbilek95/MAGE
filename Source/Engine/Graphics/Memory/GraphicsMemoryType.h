#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	enum class GraphicsMemoryType : u8
	{
		GMT_DeviceLocal = 1 << 0,
		GMT_HostVisible = 1 << 1,
		GMT_HostCoherent = 1 << 2,
		GMT_HostCached = 1 << 3
	};

	GENERATE_ENUM_FLAG(GraphicsMemoryType, u8);
}
