#include "VulkanPipeline.h"

#include "../Core/VkAssert.h"
#include "../Core/VkFunctions.h"
#include "../Device/VulkanDevice.h"
#include "../Shader/VulkanShader.h"
#include "../Descriptor/VulkanDescLayout.h"

namespace MAGE
{
	VulkanPipeline::VulkanPipeline(const GraphicsPipelineProps& desc, VulkanDevice* device) : m_props(desc),
		m_deviceRef(device), m_pipeline(VK_NULL_HANDLE), m_layout(VK_NULL_HANDLE), m_bindPoint(VK_PIPELINE_BIND_POINT_GRAPHICS)
	{
		Vector<VkPipelineShaderStageCreateInfo> shaderStages;
		for (auto& shader : desc.shaders)
		{
			VkPipelineShaderStageCreateInfo stageInfo = {};
			stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			stageInfo.stage = shader->GetShaderStage();
			stageInfo.module = shader->GetShaderModule();
			stageInfo.pName = shader->GetEntryPoint().c_str();
			shaderStages.push_back(stageInfo);
		}

		Vector<VkDescriptorSetLayout> descLayouts;
		for (auto& layout : desc.descLayouts)
		{
			descLayouts.push_back(layout->GetLayout());
		}

		VkPipelineLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		layoutInfo.setLayoutCount = static_cast<u32>(descLayouts.size());
		layoutInfo.pSetLayouts = descLayouts.data();

		ErrorUtils::VkAssert(vkCreatePipelineLayout(device->GetDevice(), &layoutInfo, nullptr, &m_layout), "VulkanPipeline");

		// Input Assembly
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

		// Input Assembly State
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

		// Viewport
		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &desc.viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &desc.scissor;

		// Rasterization
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

		// Multi sampling
		VkPipelineMultisampleStateCreateInfo multisampling = {};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		// Color Blending
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

		VkPipelineRenderingCreateInfo renderingInfo = {};
		renderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
		renderingInfo.colorAttachmentCount = desc.colorFormats.size();
		renderingInfo.pColorAttachmentFormats = desc.colorFormats.data();
		renderingInfo.depthAttachmentFormat = desc.depthFormat;
		renderingInfo.stencilAttachmentFormat = VK_FORMAT_UNDEFINED;
		renderingInfo.pNext = nullptr;
		renderingInfo.viewMask = 0;
		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = static_cast<u32>(shaderStages.size());
		pipelineInfo.pStages = shaderStages.data();
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = &depthstencil;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = nullptr;
		pipelineInfo.layout = m_layout;
		pipelineInfo.renderPass = nullptr;
		pipelineInfo.subpass = 0;
		pipelineInfo.pNext = &renderingInfo;

		ErrorUtils::VkAssert(vkCreateGraphicsPipelines(device->GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline), "VulkanPipeline");
	}

	VulkanPipeline::~VulkanPipeline()
	{
		vkDestroyPipeline(m_deviceRef->GetDevice(), m_pipeline, nullptr);
		vkDestroyPipelineLayout(m_deviceRef->GetDevice(), m_layout, nullptr);
	}
}
