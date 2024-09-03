#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Object/DeviceObject.h"
#include "Engine/Graphics/Descriptor/DescriptorType.h"

namespace MAGE
{
	/**
	 * @struct DescriptorSetUpdateEntry
	 * @brief Description of a descriptor set update entry
	 */
	struct DescriptorSetUpdateEntry
	{
		DeviceObject* pResource;
		DeviceObject* pSampler;
		DescriptorType Type;
		u32 Count;
		u32 ArrayElement;
		u64 BufferOffset;
		u32 Binding;
	};
}