#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Descriptor/DescriptorLayoutEntry.h>
#include <Engine/Graphics/Descriptor/DescriptorLayoutFlags.h>

namespace MAGE
{
	struct DescriptorLayoutDesc
	{
		DArray<DescriptorLayoutEntry> Entries;
		DescriptorLayoutFlags Flags;
	};
}