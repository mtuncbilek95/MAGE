#include "VPipeline.h"

#include "../Device/VDevice.h"
#include "../Descriptor/VDescLayout.h"
#include "../Shader/VShader.h"

#include "Engine/Vulkan/Core/VkAssert.h"
#include "Engine/Vulkan/Core/VkUtils.h"

namespace MAGE
{
	VPipeline::VPipeline(const GraphicsPipelineProps& desc, VDevice* device) : VkObject(device), m_gProps(desc), m_bindPoint(vk::PipelineBindPoint::eGraphics)
	{
		Vector<vk::DescriptorSetLayout> dLayouts(desc.layouts.size());
		for (u32 i = 0; i < dLayouts.size(); i++)
			dLayouts[i] = desc.layouts[i]->GetVkLayout();

		vk::PipelineLayoutCreateInfo layoutInfo = {};
		layoutInfo.setLayoutCount = static_cast<u32>(dLayouts.size());
		layoutInfo.pSetLayouts = dLayouts.data();

		ErrorUtils::VkAssert(m_rootDevice->GetVkDevice().createPipelineLayout(&layoutInfo, nullptr, &m_layout), "VGraphicsPipeline");

		Vector<vk::PipelineShaderStageCreateInfo> sStages;

		for (u32 i = 0; i < desc.shaderStages.size(); i++)
		{
			auto& shader = desc.shaderStages[i];
			vk::PipelineShaderStageCreateInfo info = {};
			info.module = shader->GetVkShaderModule();
			info.pName = shader->GetEntryPoint().CharString();
			info.stage = shader->GetVkShaderStage();
			sStages.push_back(info);
		}

		Vector<vk::VertexInputBindingDescription> bindings;
		Vector<vk::VertexInputAttributeDescription> attributes;
		for (u8 bindIndex = 0; bindIndex < desc.inputAssembler.bindings.size(); bindIndex++)
		{
			const auto& element = desc.inputAssembler.bindings[bindIndex];
			vk::VertexInputBindingDescription binding = {};
			binding.binding = bindIndex;
			binding.inputRate = element.inputRate;

			u32 offset = 0;
			for (u8 attIndex = 0; attIndex < element.attributes.size(); attIndex++)
			{
				vk::VertexInputAttributeDescription attr = {};
				attr.binding = bindIndex;
				attr.location = attIndex;
				attr.format = element.attributes[attIndex];
				attr.offset = offset;
				attributes.push_back(attr);

				offset += VkUtils::GetVkTextureSize(element.attributes[attIndex]);
			}
			binding.stride = offset;
			bindings.push_back(binding);
		}

		// VertexInputState
		vk::PipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.vertexBindingDescriptionCount = static_cast<u32>(bindings.size());
		vertexInputInfo.pVertexBindingDescriptions = bindings.data();
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<u32>(attributes.size());
		vertexInputInfo.pVertexAttributeDescriptions = attributes.data();

		// InputAssemblyState
		vk::PipelineInputAssemblyStateCreateInfo inputAssembly = {};
		inputAssembly.topology = desc.inputAssembler.topology;
		inputAssembly.primitiveRestartEnable = desc.inputAssembler.primitiveRestartEnable;

		Vector<vk::DynamicState> dStates;
		if (desc.viewportState.dynamicView)
			dStates.push_back(vk::DynamicState::eViewport);
		if (desc.viewportState.dynamicScissor)
			dStates.push_back(vk::DynamicState::eScissor);

		// DynamicPipelineState
		vk::PipelineDynamicStateCreateInfo dynamicState = {};
		dynamicState.dynamicStateCount = static_cast<u32>(dStates.size());
		dynamicState.pDynamicStates = dStates.data();

		// ViewportState
		vk::PipelineViewportStateCreateInfo viewportState = {};
		viewportState.viewportCount = 1;
		viewportState.pViewports = &desc.viewportState.viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &desc.viewportState.scissor;

		// RasterizationState
		vk::PipelineRasterizationStateCreateInfo rasterizer = {};
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = desc.rasterizerState.polygonMode;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = desc.rasterizerState.cullMode;
		rasterizer.frontFace = desc.rasterizerState.frontFace;
		rasterizer.depthBiasEnable = desc.rasterizerState.depthBiasEnable;
		rasterizer.depthBiasConstantFactor = desc.rasterizerState.depthBiasConstantFactor;
		rasterizer.depthBiasClamp = desc.rasterizerState.depthBiasClamp;
		rasterizer.depthBiasSlopeFactor = desc.rasterizerState.depthBiasSlopeFactor;

		// MultisampleState
		vk::PipelineMultisampleStateCreateInfo multisampling = {};
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;

		// ColorBlendAttachmentState
		Vector<vk::PipelineColorBlendAttachmentState> colorBlendAttachments;
		for (auto& attachment : desc.blendState.attachments)
		{
			vk::PipelineColorBlendAttachmentState colorBlendAttachment = {};
			colorBlendAttachment.colorWriteMask = attachment.colorWriteMask;

			colorBlendAttachment.blendEnable = attachment.blendEnable;

			colorBlendAttachment.srcColorBlendFactor = attachment.srcColorBlendFactor;
			colorBlendAttachment.dstColorBlendFactor = attachment.dstColorBlendFactor;
			colorBlendAttachment.colorBlendOp = attachment.colorBlendOp;

			colorBlendAttachment.srcAlphaBlendFactor = attachment.srcAlphaBlendFactor;
			colorBlendAttachment.dstAlphaBlendFactor = attachment.dstAlphaBlendFactor;
			colorBlendAttachment.alphaBlendOp = attachment.alphaBlendOp;

			colorBlendAttachments.push_back(colorBlendAttachment);
		}

		// ColorBlendState
		vk::PipelineColorBlendStateCreateInfo colorBlending = {};
		colorBlending.logicOpEnable = desc.blendState.logicOpEnable;
		colorBlending.logicOp = desc.blendState.logicOp;
		colorBlending.attachmentCount = static_cast<u32>(colorBlendAttachments.size());
		colorBlending.pAttachments = colorBlendAttachments.data();
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;

		// DepthStencilState
		vk::PipelineDepthStencilStateCreateInfo depthstencil = {};
		depthstencil.depthWriteEnable = desc.depthStencilState.depthWriteEnable;
		depthstencil.depthTestEnable = desc.depthStencilState.depthTestEnable;
		depthstencil.depthCompareOp = desc.depthStencilState.depthCompareOp;
		depthstencil.depthBoundsTestEnable = VK_FALSE;
		depthstencil.stencilTestEnable = VK_FALSE;
		depthstencil.minDepthBounds = 0.0f;
		depthstencil.maxDepthBounds = 1.0f;
		depthstencil.front = desc.depthStencilState.front;
		depthstencil.back = desc.depthStencilState.back;

		// Dynamic Rendering Info
		// It will be added to pNext of VkGraphicsPipelineCreateInfo
		vk::PipelineRenderingCreateInfo renderingInfo = {};
		renderingInfo.colorAttachmentCount = desc.dynamicRendering.colorAttachments.size();
		renderingInfo.pColorAttachmentFormats = desc.dynamicRendering.colorAttachments.data();
		renderingInfo.depthAttachmentFormat = desc.dynamicRendering.depthAttachment;
		renderingInfo.stencilAttachmentFormat = desc.dynamicRendering.stencilAttachment;
		renderingInfo.pNext = nullptr;
		renderingInfo.viewMask = desc.dynamicRendering.viewMask;

		vk::GraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.stageCount = static_cast<u32>(sStages.size());
		pipelineInfo.pStages = sStages.data();
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = &depthstencil;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = dStates.size() > 0 ? &dynamicState : nullptr;
		pipelineInfo.layout = m_layout;
		pipelineInfo.renderPass = nullptr;
		pipelineInfo.subpass = 0;
		pipelineInfo.flags = desc.createFlag;
		pipelineInfo.pNext = &renderingInfo;

		ErrorUtils::VkAssert(device->GetVkDevice().createGraphicsPipelines({}, 1, &pipelineInfo, nullptr, &m_pipeline), "VulkanPipeline");
	}

	VPipeline::~VPipeline()
	{
		Destroy();
	}

	void VPipeline::Destroy()
	{
		if (m_pipeline != VK_NULL_HANDLE)
		{
			m_rootDevice->GetVkDevice().destroyPipeline(m_pipeline);
			m_pipeline = VK_NULL_HANDLE;
		}

		if (m_layout != VK_NULL_HANDLE)
		{
			m_rootDevice->GetVkDevice().destroyPipelineLayout(m_layout);
			m_layout = VK_NULL_HANDLE;
		}
	}
}
