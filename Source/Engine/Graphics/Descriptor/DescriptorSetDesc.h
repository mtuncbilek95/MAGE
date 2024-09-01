#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Descriptor/DescriptorPool.h"
#include "Engine/Graphics/Descriptor/DescriptorLayout.h"

namespace MAGE
{
	struct DescriptorSetDesc
	{
		DescriptorLayout* pLayout;
		DescriptorPool* pOwnerPool;
	};
}