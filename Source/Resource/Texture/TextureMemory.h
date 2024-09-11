#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Memory/MemoryOwnedBuffer.h"

namespace MAGE
{
	struct RESOURCE_API TextureMemory
	{
		MemoryOwnedBuffer Data;
		Math::Vec2u ImageRes;
		u8 NumChannel;
	};
}
