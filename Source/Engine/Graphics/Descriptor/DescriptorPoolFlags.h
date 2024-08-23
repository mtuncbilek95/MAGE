#pragma once

#include <Engine/Core/Core.h>

namespace MAGE
{
	enum class DescriptorPoolFlags : u8
	{
		DPF_None = 1 << 0,
		FreeDescriptorSet = 1 << 1,
		UpdateAfterBind = 1 << 2,
		HostOnly = 1 << 3,
		OverallAllocationSets = 1 << 4,
		OverallAllocationPool = 1 << 5
	};

	GENERATE_ENUM_FLAG(MAGE::DescriptorPoolFlags, u8);
}