#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	/**
	 * @enum GraphicsMemoryAccessFlags
	 * @brief Flags for graphics memory access
	 */
	enum class GraphicsMemoryAccessFlags : u32
	{
		GMAF_Unknown = 0,
		GMAF_IndexRead = 1 << 0,
		GMAF_VertexAttributeRead = 1 << 1,
		GMAF_UniformRead = 1 << 2,
		GMAF_InputAttachmentRead = 1 << 3,
		GMAF_ShaderRead = 1 << 4,
		GMAF_ShaderWrite = 1 << 5,
		GMAF_ColorAttachmentRead = 1 << 6,
		GMAF_ColorAttachmentWrite = 1 << 7,
		GMAF_DepthStencilAttachmentRead = 1 << 8,
		GMAF_DepthStencilAttachmentWrite = 1 << 9,
		GMAF_TransferRead = 1 << 10,
		GMAF_TransferWrite = 1 << 11,
		GMAF_HostRead = 1 << 12,
		GMAF_HostWrite = 1 << 13,
		GMAF_MemoryRead = 1 << 14,
		GMAF_MemoryWrite = 1 << 15
	};
}
