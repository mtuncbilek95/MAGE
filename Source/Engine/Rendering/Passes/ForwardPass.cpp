#include "ForwardPass.h"

#include "Engine/Rendering/RenderContext.h"

namespace MAGE
{
	Gfx::Context& context = Gfx::Context::Get();

	ForwardPass::ForwardPass()
	{
		RenderPassProps passProps =
		{
			.hasColor = true,
			.colorPass = { VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR },
			.hasDepth = false,
			.depthPass = {},
			.clearColor = {0.1f, 0.2f, 0.3f, 1.f},
			.depthValue = { 0.f, 0.f }
		};
		m_renderPass = MakeOwned<VRenderPass>(passProps, context.GetDevice());

		for (usize i = 0; i < context.GetSwapchain()->GetImageCount(); i++)
		{
			FramebufferProps bufferProp =
			{
				.imageView = &*context.GetSwapchain()->GetImageView(i),
				.renderPass = &*m_renderPass
			};
			m_framebuffers.push_back(MakeOwned<VFramebuffer>(bufferProp, context.GetDevice()));
		}

		ShaderProps vProp =
		{
			.shaderPath = R"(D:\Projects\MAGE\Shaders\Triangle.vert)",
			.includePath = "",
			.entryPoint = "main",
			.shaderStage = VK_SHADER_STAGE_VERTEX_BIT
		};
		m_vShader = MakeOwned<VShader>(vProp, context.GetDevice());

		ShaderProps fProp =
		{
			.shaderPath = R"(D:\Projects\MAGE\Shaders\Triangle.frag)",
			.includePath = "",
			.entryPoint = "main",
			.shaderStage = VK_SHADER_STAGE_FRAGMENT_BIT
		};
		m_fShader = MakeOwned<VShader>(fProp, context.GetDevice());

		DescLayoutProps descProp = {};
		descProp.initFlags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_DESCRIPTOR_BUFFER_BIT_EXT; //VkDescriptorSetLayoutCreateFlags()
		descProp.bindings.push_back({0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT });
		m_descLayout = MakeOwned<VDescLayout>(descProp, context.GetDevice());

		GraphicsPipelineProps pipelineProps;
		pipelineProps.layouts.push_back(&*m_descLayout);
		pipelineProps.initFlag = VK_PIPELINE_CREATE_DESCRIPTOR_BUFFER_BIT_EXT; //VkPipelineCreateFlags()

		pipelineProps.shaderStages = { &*m_vShader, &*m_fShader };

		pipelineProps.inputAssembler.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		pipelineProps.inputAssembler.primitiveRestartEnable = VK_FALSE;

		InputBinding binding;
		binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		binding.attributes = { VK_FORMAT_R32G32B32_SFLOAT, VK_FORMAT_R32G32B32_SFLOAT, VK_FORMAT_R32G32B32_SFLOAT, VK_FORMAT_R32G32B32_SFLOAT, VK_FORMAT_R32G32B32A32_SFLOAT, VK_FORMAT_R32G32_SFLOAT };

		pipelineProps.inputAssembler.bindings = { binding };

		pipelineProps.viewportState.dynamicView = VK_FALSE;
		pipelineProps.viewportState.viewport = VkViewport{
			0.0f, 0.0f,
			1280.f, 720.f,
			0.0f, 1.0f
		};
		pipelineProps.viewportState.dynamicScissor = VK_FALSE;
		pipelineProps.viewportState.scissor = VkRect2D{
			{0, 0},  // offset (x, y)
			{1280, 720}  // extent (width, height)
		};

		pipelineProps.rasterizerState.depthBiasEnable = VK_FALSE;
		pipelineProps.rasterizerState.depthBiasConstantFactor = 0.0f;
		pipelineProps.rasterizerState.depthBiasClamp = 0.0f;
		pipelineProps.rasterizerState.depthBiasSlopeFactor = 0.0f;
		pipelineProps.rasterizerState.polygonMode = VK_POLYGON_MODE_FILL;
		pipelineProps.rasterizerState.cullMode = VK_CULL_MODE_NONE;
		pipelineProps.rasterizerState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

		BlendStateAttachment blendAttachment;
		blendAttachment.blendEnable = VK_FALSE;
		blendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
		blendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
		blendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		blendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		blendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		blendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
		blendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
			VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

		pipelineProps.blendState.logicOpEnable = VK_FALSE;
		pipelineProps.blendState.logicOp = VK_LOGIC_OP_COPY;
		pipelineProps.blendState.attachments = { blendAttachment };

		pipelineProps.depthStencilState.depthTestEnable = VK_FALSE;
		pipelineProps.depthStencilState.depthWriteEnable = VK_FALSE;
		pipelineProps.depthStencilState.depthCompareOp = VK_COMPARE_OP_ALWAYS;
		pipelineProps.depthStencilState.depthBoundsTestEnable = VK_FALSE;
		pipelineProps.depthStencilState.stencilTestEnable = VK_FALSE;
		pipelineProps.depthStencilState.front = {};
		pipelineProps.depthStencilState.back = {};
		pipelineProps.depthStencilState.minDepthBounds = 0.0f;
		pipelineProps.depthStencilState.maxDepthBounds = 1.0f;

		m_pipeline = MakeOwned<VPipeline>(pipelineProps, context.GetDevice());
	}

	ForwardPass::~ForwardPass()
	{
		m_renderPass->Destroy();
		m_framebuffers.clear();

		//m_pipeline->Destroy();

		//m_vShader->Destroy();
		//m_fShader->Destroy();
	}
}
