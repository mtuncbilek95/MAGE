/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "../Abstraction/VObject.h"

namespace MAGE
{
	class VDescLayout;
	class VShader;
	class VRenderPass;

	struct InputBinding final
	{
		VkVertexInputRate inputRate;
		Vector<VkFormat> attributes;
	};

	struct InputAssembler final
	{
		VkPrimitiveTopology topology;
		b8 primitiveRestartEnable;
		Vector<InputBinding> bindings;
	};

	struct ViewportState final
	{
		b8 dynamicView;
		VkViewport viewport;
		b8 dynamicScissor;
		VkRect2D scissor;
	};

	struct RasterizerState final
	{
		b8 depthBiasEnable;
		f32 depthBiasConstantFactor;
		f32 depthBiasClamp;
		f32 depthBiasSlopeFactor;
		VkPolygonMode polygonMode;
		VkCullModeFlags cullMode;
		VkFrontFace frontFace;
	};

	struct BlendStateAttachment final
	{
		b8 blendEnable;
		VkBlendFactor srcColorBlendFactor;
		VkBlendFactor dstColorBlendFactor;
		VkBlendOp colorBlendOp;
		VkBlendFactor srcAlphaBlendFactor;
		VkBlendFactor dstAlphaBlendFactor;
		VkBlendOp alphaBlendOp;
		VkColorComponentFlags colorWriteMask;
	};

	struct BlendState final
	{
		b8 logicOpEnable;
		VkLogicOp logicOp;
		Vector<BlendStateAttachment> attachments;
	};

	struct DepthStencilState final
	{
		b8 depthTestEnable;
		b8 depthWriteEnable;
		VkCompareOp depthCompareOp;
		b8 depthBoundsTestEnable;
		b8 stencilTestEnable;
		VkStencilOpState front;
		VkStencilOpState back;
		f32 minDepthBounds;
		f32 maxDepthBounds;
	};

	struct GraphicsPipelineProps final
	{
		Vector<VDescLayout*> layouts;
		Vector<VShader*> shaderStages;
		InputAssembler inputAssembler;
		ViewportState viewportState;
		RasterizerState rasterizerState;
		BlendState blendState;
		DepthStencilState depthStencilState;
		VRenderPass* renderPass;
	};

	class VPipeline final : public VObject
	{
		VPipeline(const GraphicsPipelineProps& desc, VDevice* device);
		~VPipeline() override final;

		inline VkPipelineLayout GetLayout() const { return m_layout; }
		inline VkPipeline GetPipeline() const { return m_pipeline; }

		void Destroy() override final;

	private:
		GraphicsPipelineProps m_grapProps;

		VkPipelineLayout m_layout;
		VkPipeline m_pipeline;

		VkPipelineBindPoint m_bindPoint;
	};
}