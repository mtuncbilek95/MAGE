#pragma once 

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Descriptor/DescriptorLayout.h>
#include <Engine/Vulkan/Device/VDevice.h>

#include <vulkan/vulkan.h>

namespace MAGE
{
	class VDescriptorLayout final : public DescriptorLayout
	{
	public:
		VDescriptorLayout(const DescriptorLayoutDesc& desc, VDevice* pDevice);
		~VDescriptorLayout() override = default;

		VkDescriptorSetLayout GetVkDescriptorLayout() const { return mVkDescriptorSetLayout; }
		VkDevice GetVkDevice() const { return mDevice; }

	private:
		VkDescriptorSetLayout mVkDescriptorSetLayout;
		VkDevice mDevice;
	};
}
