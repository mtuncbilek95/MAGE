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
		vk::VertexInputRate inputRate = vk::VertexInputRate::eVertex;
		Vector<vk::Format> attributes;
	};

	struct InputAssembler final
	{
		vk::PrimitiveTopology topology = vk::PrimitiveTopology::eTriangleList;
		b8 primitiveRestartEnable = false;
		Vector<InputBinding> bindings;
	};

	struct ViewportState final
	{
		b8 dynamicView = false;
		vk::Viewport viewport;
		b8 dynamicScissor = false;
		vk::Rect2D scissor;
	};

	struct RasterizerState final
	{
		b8 depthBiasEnable = true;
		f32 depthBiasConstantFactor = 0.0f;
		f32 depthBiasClamp = 1.0f;
		f32 depthBiasSlopeFactor = 0.0f;
		vk::PolygonMode polygonMode = vk::PolygonMode::eFill;
		vk::CullModeFlags cullMode = vk::CullModeFlagBits::eFront;
		vk::FrontFace frontFace = vk::FrontFace::eClockwise;
	};

	struct BlendStateAttachment final
	{
		b8 blendEnable = true;
		vk::BlendFactor srcColorBlendFactor = vk::BlendFactor::eZero;
		vk::BlendFactor dstColorBlendFactor = vk::BlendFactor::eOne;
		vk::BlendOp colorBlendOp = vk::BlendOp::eAdd;
		vk::BlendFactor srcAlphaBlendFactor = vk::BlendFactor::eZero;
		vk::BlendFactor dstAlphaBlendFactor = vk::BlendFactor::eOne;
		vk::BlendOp alphaBlendOp = vk::BlendOp::eAdd;
		vk::ColorComponentFlags colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
	};

	struct BlendState final
	{
		b8 logicOpEnable = true;
		vk::LogicOp logicOp = vk::LogicOp::eCopy;
		Vector<BlendStateAttachment> attachments;
	};

	struct DepthStencilState final
	{
		b8 depthTestEnable = false;
		b8 depthWriteEnable = false;
		vk::CompareOp depthCompareOp = vk::CompareOp::eAlways;
		b8 depthBoundsTestEnable = false;
		b8 stencilTestEnable = false;
		vk::StencilOpState front = vk::StencilOpState();
		vk::StencilOpState back = vk::StencilOpState();
		f32 minDepthBounds = 0.0f;
		f32 maxDepthBounds = 1.0f;
	};

	struct DynamicAttachments final
	{
		Vector<vk::Format> colorAttachments;
		vk::Format depthAttachment = vk::Format::eUndefined;
		vk::Format stencilAttachment = vk::Format::eUndefined;
		u32 viewMask = 0;
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
		DynamicAttachments dynamicRendering;

		vk::PipelineCreateFlags createFlag;
	};

	class VPipeline final : public VkObject
	{
	public:
		VPipeline(const GraphicsPipelineProps& desc, VDevice* device);
		~VPipeline() override final;

		inline vk::PipelineLayout GetVkLayout() const { return m_layout; }
		inline vk::Pipeline GetVkPipeline() const { return m_pipeline; }
		inline vk::PipelineBindPoint GetVkBindPoint() const { return m_bindPoint; }

		void Destroy() override final;

	private:
		GraphicsPipelineProps m_gProps;

		vk::PipelineLayout m_layout;
		vk::Pipeline m_pipeline;

		vk::PipelineBindPoint m_bindPoint;
	};
}
