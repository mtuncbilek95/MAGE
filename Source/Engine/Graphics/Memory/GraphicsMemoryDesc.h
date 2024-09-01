#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Memory/GraphicsMemoryType.h"

namespace MAGE
{
	struct GraphicsMemoryDesc
	{
		GraphicsMemoryType MemoryType;
		u64 MemorySize;
	};
}
