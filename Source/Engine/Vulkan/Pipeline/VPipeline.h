/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Vulkan/Core/VkObject.h"
#include <vulkan/vulkan.hpp>

namespace MAGE
{
	class VDescLayout;
	class VShader;

	struct InputBinding final
	{
		VkVertexInputRate inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		vector<VkFormat> attributes;
	};

	struct InputAssembler final
	{
		VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		b8 primitiveRestartEnable = false;
		vector<InputBinding> bindings;
	};

	struct ViewportState final
	{
		b8 dynamicView = false;
		VkViewport viewport;
		b8 dynamicScissor = false;
		VkRect2D scissor;
	};

	struct RasterizerState final
	{
		b8 depthBiasEnable = true;
		f32 depthBiasConstantFactor = 0.0f;
		f32 depthBiasClamp = 1.0f;
		f32 depthBiasSlopeFactor = 0.0f;
		VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL;
		VkCullModeFlags cullMode = VK_CULL_MODE_FRONT_BIT;
		VkFrontFace frontFace = VK_FRONT_FACE_CLOCKWISE;
	};

	struct BlendStateAttachment final
	{
		b8 blendEnable = true;
		VkBlendFactor srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		VkBlendFactor dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
		VkBlendOp colorBlendOp = VK_BLEND_OP_ADD;
		VkBlendFactor srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		VkBlendFactor dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		VkBlendOp alphaBlendOp = VK_BLEND_OP_ADD;
		VkColorComponentFlags colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	};

	struct BlendState final
	{
		b8 logicOpEnable = true;
		VkLogicOp logicOp = VK_LOGIC_OP_COPY;
		vector<BlendStateAttachment> attachments;
	};

	struct DepthStencilState final
	{
		b8 depthTestEnable = false;
		b8 depthWriteEnable = false;
		VkCompareOp depthCompareOp = VK_COMPARE_OP_ALWAYS;
		b8 depthBoundsTestEnable = false;
		b8 stencilTestEnable = false;
		VkStencilOpState front = {};
		VkStencilOpState back = {};
		f32 minDepthBounds = 0.0f;
		f32 maxDepthBounds = 1.0f;
	};

	struct DynamicAttachments final
	{
		vector<VkFormat> colorAttachments;
		VkFormat depthAttachment = VK_FORMAT_UNDEFINED;
		VkFormat stencilAttachment = VK_FORMAT_UNDEFINED;
		u32 viewMask = 0;
	};

	struct GraphicsPipelineProps final
	{
		vector<VDescLayout*> layouts;
		vector<VShader*> shaderStages;
		InputAssembler inputAssembler;
		ViewportState viewportState;
		RasterizerState rasterizerState;
		BlendState blendState;
		DepthStencilState depthStencilState;
		DynamicAttachments dynamicRendering;

		VkPipelineCreateFlags createFlag;
	};

	class VPipeline final : public VkObject
	{
	public:
		VPipeline(const GraphicsPipelineProps& desc, VDevice* device);
		~VPipeline() override final;

		inline VkPipelineLayout GetVkLayout() const { return m_layout; }
		inline VkPipeline GetVkPipeline() const { return m_pipeline; }
		inline VkPipelineBindPoint GetVkBindPoint() const { return m_bindPoint; }

		void Destroy() override final;

	private:
		GraphicsPipelineProps m_gProps;

		VkPipelineLayout m_layout;
		VkPipeline m_pipeline;

		VkPipelineBindPoint m_bindPoint;
	};
}