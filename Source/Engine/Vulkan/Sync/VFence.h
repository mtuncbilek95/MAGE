#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Sync/Fence.h>

#include <vulkan/vulkan.h>

namespace MAGE
{
	class VDevice;
	class VFence : public Fence
	{
	public:
		VFence(b8 bSignalled, VDevice* pDevice);
		~VFence() override = default;

		VkFence GetVkFence() const { return mFence; }

	private:
		VkFence mFence;
		VkDevice mDevice;
	};
}
