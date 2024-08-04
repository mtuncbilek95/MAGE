#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Pipeline/Pipeline.h>

#include <vulkan/vulkan.h>

namespace MAGE
{
	class VDevice;
	class VPipeline final : public Pipeline
	{
	public:
		VPipeline(const GraphicsPipelineDesc& desc, VDevice* pDevice);
		~VPipeline() override = default;

		VkPipeline GetVkPipeline() const { return mVkPipeline; }
		VkPipelineLayout GetVkPipelineLayout() const { return mVkPipelineLayout; }

	private:
		VkPipeline mVkPipeline;
		VkPipelineLayout mVkPipelineLayout;
		VkDevice mVkDevice;
	};
}
