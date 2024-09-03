#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Descriptor/DescriptorType.h"

namespace MAGE
{
	/**
	 * @struct DescriptorPoolSize
	 * @brief Description of a descriptor pool size
	 */
	struct DescriptorPoolSize
	{
		DescriptorType Type;
		u64 Count;
	};
}