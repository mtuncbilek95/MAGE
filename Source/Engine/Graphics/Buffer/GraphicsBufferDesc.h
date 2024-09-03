#pragma once 

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Buffer/GraphicsBufferUsage.h"

namespace MAGE
{
	class GraphicsMemory;

	/**
	 * @struct GraphicsBufferDesc
	 * @brief Description of a graphics buffer
	 */
	struct GraphicsBufferDesc
	{
		u64 BlockSize;
		u64 BlockLength;
		GraphicsBufferUsage Usage;
		GraphicsMemory* pRequestMemory;
	};
}