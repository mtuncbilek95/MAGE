#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Descriptor/DescriptorSet.h"
#include "Engine/Vulkan/Device/VDevice.h"

#include <vulkan/vulkan.h>

namespace MAGE
{
	class VDescriptorSet final : public DescriptorSet
	{
	public:
		VDescriptorSet(const DescriptorSetDesc& desc, VDevice* pDevice);
		~VDescriptorSet() override = default;

		VkDescriptorSet GetVkDescriptorSet() const { return mVkDescriptorSet; }
		VkDevice GetVkDevice() const { return mDevice; }

	private:
		VkDescriptorSet mVkDescriptorSet;
		VkDescriptorPool mVkDescriptorPool;
		VkDescriptorSetLayout mVkDescriptorLayout;
		VkDevice mDevice;
	};
}
