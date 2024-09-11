#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Command/CmdPool.h"

#include <vulkan/vulkan.h>

namespace MAGE
{
	class VDevice;
	class ENGINE_API VCmdPool : public CmdPool
	{
	public:
		VCmdPool(const CmdPoolDesc& desc, VDevice* pDevice);
		~VCmdPool() override = default;

		VkCommandPool GetVkCommandPool() const { return mVkCommandPool; }

	private:
		VkCommandPool mVkCommandPool;
		VkDevice mDevice;
	};
}
