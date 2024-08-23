#pragma once

#include <Engine/Core/Core.h>

namespace MAGE
{
	enum class GraphicsMemoryType : u8
	{
		DeviceLocal = 1 << 0,
		HostVisible = 1 << 1,
		HostCoherent = 1 << 2,
		HostCached = 1 << 3
	};

	GENERATE_ENUM_FLAG(MAGE::GraphicsMemoryType, u8);
}
