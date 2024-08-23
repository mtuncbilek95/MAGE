#pragma once

#include <Engine/Core/Core.h>

namespace MAGE
{
	enum class DescriptorLayoutFlags : u8
	{
        DLF_None = 0,
        PushDescriptor = 1 << 0,
        UpdateAfterBind = 1 << 1,
        HostOnly = 1 << 2,
        BufferBit = 1 << 3,
        ImmutableSamplers = 1 << 4,
        PerStageDescriptor = 1 << 5,
        IndirectBindable = 1 << 6,
	};

	GENERATE_ENUM_FLAG(MAGE::DescriptorLayoutFlags, u8);
}
