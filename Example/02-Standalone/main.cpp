#include <Engine/Core/Core.h>
#include <Engine/Window/WindowManager.h>
#include <Engine/Rendering/RenderContext.h>

#include <Engine/VulkanRHI/Command/VCmdPool.h>
#include <Engine/VulkanRHI/Command/VCmdBuffer.h>

#include <Engine/VulkanRHI/Shader/VShader.h>
#include <Engine/VulkanRHI/Pipeline/VPipeline.h>

#include <Engine/VulkanRHI/Buffer/VStageBuffer.h>
#include <Engine/VulkanRHI/Buffer/VDstBuffer.h>

using namespace MAGE;

struct Vertex {
	Math::Vec3f Pos;  // Vertex position (x, y, z)
};

std::vector<Vertex> triangle = {
	{{  0.0f, -0.5f,  0.0f }},  // Bottom vertex
	{{  0.5f,  0.5f,  0.0f }},  // Top-right vertex
	{{ -0.5f,  0.5f,  0.0f }}   // Top-left vertex
};

int main(int argC, char** argV)
{
	SystemLog::Get().InitLogger<ConsoleSink>();

	IndWindowDesc windowProps =
	{
		.windowRes = {1920, 1080},
		.mode = WindowMode::Windowed,
		.title = "TestWindow"
	};
	Manager::Window::Get().InitWindow(windowProps);
	auto& window = Manager::Window::Get().GetWindow();

	auto& context = Gfx::Context::Get();
	context.Init();

	CmdPoolProps poolProp =
	{
		.queue = context.GetGraphicsQueue(),
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
	};
	auto pool = MakeOwned<VCmdPool>(poolProp, context.GetDevice());
	auto cmdBuffer = pool->CreateCmdBuffer(VK_COMMAND_BUFFER_LEVEL_SECONDARY);
	auto primBuffer = pool->CreateCmdBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

	ShaderProps vProp =
	{
		.shaderPath = R"(D:\Projects\MAGE\Source\Shaders\Triangle.vert)",
		.includePath = "",
		.entryPoint = "main",
		.shaderStage = VK_SHADER_STAGE_VERTEX_BIT
	};
	auto vShader = MakeOwned<VShader>(vProp, context.GetDevice());

	ShaderProps fProp =
	{
		.shaderPath = R"(D:\Projects\MAGE\Source\Shaders\Triangle.frag)",
		.includePath = "",
		.entryPoint = "main",
		.shaderStage = VK_SHADER_STAGE_FRAGMENT_BIT
	};
	auto fShader = MakeOwned<VShader>(fProp, context.GetDevice());

	GraphicsPipelineProps pipelineProps;

	pipelineProps.shaderStages = { &*vShader, &*fShader };

	pipelineProps.inputAssembler.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	pipelineProps.inputAssembler.primitiveRestartEnable = VK_FALSE;

	InputBinding binding;
	binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	binding.attributes = { VK_FORMAT_R32G32B32_SFLOAT };

	pipelineProps.inputAssembler.bindings = { binding };

	pipelineProps.viewportState.dynamicView = VK_FALSE;
	pipelineProps.viewportState.viewport = VkViewport{
		0.0f, 0.0f,
		1920.f, 1080.f,
		0.0f, 1.0f
	};
	pipelineProps.viewportState.dynamicScissor = VK_FALSE;
	pipelineProps.viewportState.scissor = VkRect2D{
		{0, 0},  // offset (x, y)
		{1920, 1080}  // extent (width, height)
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

	pipelineProps.renderPass = context.GetSwapchain()->GetRenderPass();
	auto pipeline = MakeOwned<VPipeline>(pipelineProps, context.GetDevice());

	StageBufferProps stageProps =
	{
		.sizeInBytes = sizeof(Vertex) * triangle.size()
	};
	auto sBuffer = MakeOwned<VStageBuffer>(stageProps, context.GetDevice());

	DstBufferProps dstProps =
	{
		.sizeInBytes = sizeof(Vertex) * triangle.size(),
		.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
	};
	auto dstBuffer = MakeOwned<VDstBuffer>(dstProps, context.GetDevice());

	sBuffer->MapMemory({ triangle.data(), sizeof(Vertex) * triangle.size() });



	window.Show();
	while (!window.IsClosed())
	{
		window.PollEvents();
		context.PrepareFrame();
		cmdBuffer->BeginRecording(context.GetSwapchain()->GetRenderPass(), context.GetSwapchain()->GetFramebuffer());

		cmdBuffer->EndRecording();
		context.Execute(&*cmdBuffer);
		context.SubmitFrame();
	}

	context.Shutdown();

	return 0;
}