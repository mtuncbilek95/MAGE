#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	/**
	 * @enum DescriptorLayoutFlags
	 * @brief Enum for descriptor layout flags
	 */
	enum class ENGINE_API DescriptorLayoutFlags : u8
	{
        DLF_None = 0,
        DLF_PushDescriptor = 1 << 0,
        DLF_UpdateAfterBind = 1 << 1,
        DLF_HostOnly = 1 << 2,
        DLF_BufferBit = 1 << 3,
        DLF_ImmutableSamplers = 1 << 4,
        DLF_PerStageDescriptor = 1 << 5,
        DLF_IndirectBindable = 1 << 6,
	};
}
