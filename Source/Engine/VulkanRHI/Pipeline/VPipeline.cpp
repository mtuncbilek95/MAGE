#include "VPipeline.h"

#include "../Core/VAssert.h"
#include "../Core/VLoadFuncs.h"
#include "../Device/VDevice.h"
#include "../Descriptor/VDescLayout.h"
#include "../Shader/VShader.h"
#include "../RenderPass/VRenderPass.h"

namespace MAGE
{
	VPipeline::VPipeline(const GraphicsPipelineProps& desc, VDevice* device) : VObject(device), m_grapProps(desc), m_bindPoint(VK_PIPELINE_BIND_POINT_GRAPHICS)
	{
		Vector<VkDescriptorSetLayout> dLayouts(desc.layouts.size(), VK_NULL_HANDLE);
		for (u32 i = 0; i < dLayouts.size(); i++)
			dLayouts[i] = desc.layouts[i]->GetLayout();

		VkPipelineLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		layoutInfo.setLayoutCount = static_cast<u32>(dLayouts.size());
		layoutInfo.pSetLayouts = dLayouts.data();
		layoutInfo.flags = VkPipelineLayoutCreateFlags();

		ErrorUtils::VkAssert(vkCreatePipelineLayout(m_rootDevice->GetDevice(), &layoutInfo, nullptr, &m_layout), "VGraphicsPipeline");

		Vector<VkPipelineShaderStageCreateInfo> sStages;

		for (u32 i = 0; i < desc.shaderStages.size(); i++)
		{
			auto& shader = desc.shaderStages[i];
			VkPipelineShaderStageCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			info.module = shader->GetShaderModule();
			info.pName = shader->GetEntryPoint().c_str();
			info.stage = shader->GetShaderStage();
			sStages.push_back(info);
		}

		Vector<VkVertexInputBindingDescription> bindings;
		Vector<VkVertexInputAttributeDescription> attributes;
		for (u8 bindIndex = 0; bindIndex < desc.inputAssembler.bindings.size(); bindIndex++)
		{
			const auto& element = desc.inputAssembler.bindings[bindIndex];
			VkVertexInputBindingDescription binding = {};
			binding.binding = bindIndex;
			binding.inputRate = element.inputRate;

			u32 offset = 0;
			for (u8 attIndex = 0; attIndex < element.attributes.size(); attIndex++)
			{
				VkVertexInputAttributeDescription attr = {};
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

		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = static_cast<u32>(bindings.size());
		vertexInputInfo.pVertexBindingDescriptions = bindings.data();
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<u32>(attributes.size());
		vertexInputInfo.pVertexAttributeDescriptions = attributes.data();

		VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = desc.inputAssembler.topology;
		inputAssembly.primitiveRestartEnable = desc.inputAssembler.primitiveRestartEnable;

		Vector<VkDynamicState> dStates;
		if (desc.viewportState.dynamicView)
			dStates.push_back(VK_DYNAMIC_STATE_VIEWPORT);
		if (desc.viewportState.dynamicScissor)
			dStates.push_back(VK_DYNAMIC_STATE_SCISSOR);

		VkPipelineDynamicStateCreateInfo dynamicState = {};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<u32>(dStates.size());
		dynamicState.pDynamicStates = dStates.data();

		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &desc.viewportState.viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &desc.viewportState.scissor;

		VkPipelineRasterizationStateCreateInfo rasterizer = {};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
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

		VkPipelineMultisampleStateCreateInfo multisampling = {};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		Vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments;
		for (auto& attachment : desc.blendState.attachments)
		{
			VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
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

		VkPipelineColorBlendStateCreateInfo colorBlending = {};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = desc.blendState.logicOpEnable;
		colorBlending.logicOp = desc.blendState.logicOp;
		colorBlending.attachmentCount = static_cast<u32>(colorBlendAttachments.size());
		colorBlending.pAttachments = colorBlendAttachments.data();
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;

		VkPipelineDepthStencilStateCreateInfo depthstencil = {};
		depthstencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthstencil.depthWriteEnable = desc.depthStencilState.depthWriteEnable;
		depthstencil.depthTestEnable = desc.depthStencilState.depthTestEnable;
		depthstencil.depthCompareOp = desc.depthStencilState.depthCompareOp;
		depthstencil.depthBoundsTestEnable = VK_FALSE;
		depthstencil.stencilTestEnable = VK_FALSE;
		depthstencil.minDepthBounds = 0.0f;
		depthstencil.maxDepthBounds = 1.0f;
		depthstencil.front = desc.depthStencilState.front;
		depthstencil.back = desc.depthStencilState.back;

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
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
		pipelineInfo.renderPass = desc.renderPass->GetRenderPass();
		pipelineInfo.subpass = 0;
		pipelineInfo.pNext = nullptr;

		ErrorUtils::VkAssert(vkCreateGraphicsPipelines(device->GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline), "VulkanPipeline");
	}

	VPipeline::~VPipeline()
	{
		Destroy();
	}

	void VPipeline::Destroy()
	{
		if (m_pipeline != VK_NULL_HANDLE)
		{
			vkDestroyPipeline(m_rootDevice->GetDevice(), m_pipeline, nullptr);
			m_pipeline = VK_NULL_HANDLE;
		}

		if (m_layout != VK_NULL_HANDLE)
		{
			vkDestroyPipelineLayout(m_rootDevice->GetDevice(), m_layout, nullptr);
			m_layout = VK_NULL_HANDLE;
		}
	}
}
