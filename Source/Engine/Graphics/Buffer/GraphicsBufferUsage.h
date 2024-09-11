#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	/**
	 * @enum GraphicsBufferUsage
	 * @brief Enum for graphics buffer usage
	 */
	enum class ENGINE_API GraphicsBufferUsage : u8
	{
		GBU_Vertex = 1 << 0,
		GBU_Index = 1 << 1,
		GBU_Uniform = 1 << 2,
		GBU_Storage = 1 << 3,
		GBU_TransferSrc = 1 << 4,
		GBU_TransferDst = 1 << 5,
	};
}
