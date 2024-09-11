#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	enum class ENGINE_API TextureCreateFlags : u32
	{
		TCF_Undefined = 0,
		TCF_SparseBinding = 1 << 0,
		TCF_SparseResidency = 1 << 1,
		TCF_SparseAliased = 1 << 2,
		TCF_MutableFormat = 1 << 3,
		TCF_CubeCompatible = 1 << 4,
		TCF_Array2D = 1 << 5,
		TCF_SplitInstanceBindRegions = 1 << 6,
		TCF_BlockTexelView = 1 << 7,
		TCF_ExtendedUsage = 1 << 8,
		TCF_Disjoint = 1 << 9,
		TCF_Alias = 1 << 10,
		TCF_Protected = 1 << 11
	};
}
