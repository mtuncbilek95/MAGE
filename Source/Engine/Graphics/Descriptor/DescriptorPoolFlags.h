#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	/**
	 * @enum DescriptorPoolFlags
	 * @brief Flags for descriptor pool
	 */
	enum class DescriptorPoolFlags : u8
	{
		DPF_None = 0,
		DPF_FreeDescriptorSet = 1 << 0,
		DPF_UpdateAfterBind = 1 << 1,
		DPF_HostOnly = 1 << 2,
		DPF_OverallAllocationSets = 1 << 3,
		DPF_OverallAllocationPool = 1 << 4
	};
}