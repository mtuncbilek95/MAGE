#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Descriptor/DescriptorSetUpdateEntry.h>

namespace MAGE
{
	struct DescriptorUpdateDesc
	{
		DArray<DescriptorSetUpdateEntry> Entries;
	};
}