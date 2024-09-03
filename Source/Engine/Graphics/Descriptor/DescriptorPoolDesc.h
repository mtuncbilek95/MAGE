#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Memory/GraphicsMemoryType.h"
#include "Engine/Graphics/Descriptor/DescriptorPoolSize.h"
#include "Engine/Graphics/Descriptor/DescriptorPoolFlags.h"

namespace MAGE
{
	/**
	 * @struct DescriptorPoolDesc
	 * @brief Description of a descriptor pool
	 */
	struct DescriptorPoolDesc
	{
		u64 MaxSets;
		Vector<DescriptorPoolSize> PoolSizes;
		DescriptorPoolFlags Flags;
	};
}