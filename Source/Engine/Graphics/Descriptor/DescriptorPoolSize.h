#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Descriptor/DescriptorType.h>

namespace MAGE
{
	struct DescriptorPoolSize
	{
		DescriptorType Type;
		u64 Count;
	};
}