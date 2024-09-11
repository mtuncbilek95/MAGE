#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	/**
	 * @enum DescriptorSetFlags
	 * @brief Flags for a descriptor set
	 */
	enum class ENGINE_API DescriptorSetFlags : u8
	{
		DSF_Undefined = 0,
		DSF_UpdateAfterBind = 1 << 0,
		DSF_UpdateUnusedWhilePending = 1 << 1,
		DSF_PartiallyBound = 1 << 2,
		DSF_VariableDescriptorCount = 1 << 3
	};
}
