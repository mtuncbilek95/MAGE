#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Memory/GraphicsMemoryType.h"

namespace MAGE
{
	/**
	 * @struct GraphicsMemoryDesc
	 * @brief Description of a graphics memory
	 */
	struct GraphicsMemoryDesc
	{
		GraphicsMemoryType MemoryType;
		u64 MemorySize;
	};
}
