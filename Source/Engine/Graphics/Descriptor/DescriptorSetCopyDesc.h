#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Descriptor/DescriptorSetCopyEntry.h"

namespace MAGE
{
	/**
	 * @struct DescriptorSetCopyDesc
	 * @brief Description of a descriptor set copy
	 */
	struct DescriptorSetCopyDesc
	{
		Vector<DescriptorSetCopyEntry> Entries;
	};
}