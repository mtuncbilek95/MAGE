#pragma once

#include <Engine/Core/Core.h>

namespace MAGE
{
	struct DescriptorSetCopyEntry
	{
		u32 SourceBinding;
		u32 SourceArrayElement;

		u32 DestinationBinding;
		u32 DestinationArrayElement;

		u32 DescriptorCount;
	};
}