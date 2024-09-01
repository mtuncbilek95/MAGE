#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	enum class GraphicsBufferUsage : u8
	{
		GBU_Vertex = 1 << 0,
		GBU_Index = 1 << 1,
		GBU_Uniform = 1 << 2,
		GBU_Storage = 1 << 3,
		GBU_TransferSrc = 1 << 4,
		GBU_TransferDst = 1 << 5,
	};

	GENERATE_ENUM_FLAG(GraphicsBufferUsage, u8);
}
