#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Descriptor/DescriptorLayoutEntry.h>
#include <Engine/Graphics/Descriptor/DescriptorLayoutFlags.h>

namespace MAGE
{
	struct DescriptorLayoutDesc
	{
		Vector<DescriptorLayoutEntry> Entries;
		DescriptorLayoutFlags Flags;
	};
}