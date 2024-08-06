#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Memory/MemoryOwnedBuffer.h>

namespace MAGE
{
	struct TextureMemory
	{
		MemoryOwnedBuffer Data;
		Vec2u ImageRes;
		u8 NumChannel;
	};
}
