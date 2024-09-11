#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Memory/GraphicsMemory.h"

#include <vulkan/vulkan.h>

namespace MAGE
{
	class VDevice;
	class ENGINE_API VMemory : public GraphicsMemory
	{
	public:
		VMemory(const GraphicsMemoryDesc& desc, VDevice* pDevice);
		~VMemory() override = default;

		VkDeviceMemory GetVkDeviceMemory() const { return mVkDeviceMemory; }

	protected:
		u32 FindMemoryType(GraphicsMemoryType memoryType);

	private:
		VkDeviceMemory mVkDeviceMemory;
		VkPhysicalDevice mVkAdapter;
		VkDevice mDevice;
	};
}
