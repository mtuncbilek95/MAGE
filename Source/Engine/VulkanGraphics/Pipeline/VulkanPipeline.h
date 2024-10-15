/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include <vulkan/vulkan.h>

#include "BlendState.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"
#include "InputAssembler.h"

namespace MAGE
{
	class VulkanDevice;
	class VulkanDescLayout;
	class VulkanShader;

	struct GraphicsPipelineProps final
	{
		BlendState blendState;
		DepthStencilState depthStencilState;
		RasterizerState rasterizerState;
		InputAssembler inputAssembler;
		Vector<VulkanDescLayout*> descLayouts;
		Vector<VkFormat> colorFormats;
		VkFormat depthFormat;
		Vector<VulkanShader*> shaders;
		VkViewport viewport;
		VkRect2D scissor;
	};

	class VulkanPipeline final
	{
	public:
		VulkanPipeline(const GraphicsPipelineProps& desc, VulkanDevice* device);
		~VulkanPipeline();

		VkPipeline GetPipeline() const { return m_pipeline; }
		VkPipelineLayout GetLayout() const { return m_layout; }

	private:
		VkPipeline m_pipeline;
		VkPipelineLayout m_layout;

		VulkanDevice* m_deviceRef;

		GraphicsPipelineProps m_props;
		VkPipelineBindPoint m_bindPoint;
	};
}