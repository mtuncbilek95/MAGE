#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Descriptor/DescriptorSetUpdateEntry.h"

namespace MAGE
{
	/**
	 * @struct DescriptorUpdateDesc
	 * @brief Description of a descriptor update
	 */
	struct DescriptorUpdateDesc
	{
		Vector<DescriptorSetUpdateEntry> Entries;
	};
}