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
	Math::Vec3f Pos;
	Math::Vec3f Color;
};

Vector<Vertex> triangle = {
	{{  0.0f,  0.5f,  0.0f }, { 1.0f, 0.0f, 0.0f }},
	{{  0.5f, -0.5f,  0.0f }, { 0.0f, 1.0f, 0.0f }},
	{{ -0.5f, -0.5f,  0.0f }, { 0.0f, 0.0f, 1.0f }}
};

Vector<u32> indices = { 0, 1, 2 };

int main(int argC, char** argV)
{
	SystemLog::Get().InitLogger<ConsoleSink>();

	IndWindowDesc windowProps =
	{
		.windowRes = {1280, 720},
		.mode = WindowMode::Windowed,
		.title = "TestWindow"
	};
	Manager::Window::Get().InitWindow(windowProps);
	auto& window = Manager::Window::Get().GetWindow();

	auto& context = Gfx::Context::Get();
	context.Init();

	CmdPoolProps gPoolProp =
	{
		.queue = context.GetGraphicsQueue(),
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
	};
	CmdPoolProps tPoolProp =
	{
		.queue = context.GetTransferQueue(),
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
	};
	auto pool = MakeOwned<VCmdPool>(gPoolProp, context.GetDevice());
	auto transferPool = MakeOwned<VCmdPool>(tPoolProp, context.GetDevice());
	auto cmdBuffer = pool->CreateCmdBuffer(VK_COMMAND_BUFFER_LEVEL_SECONDARY);
	auto transferBuffer = transferPool->CreateCmdBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

	auto testSem = context.GetDevice()->CreateSyncSemaphore();

	StageBufferProps stageVertProps =
	{
		.sizeInBytes = sizeof(Vertex) * triangle.size()
	};
	StageBufferProps stageIndProps =
	{
		.sizeInBytes = sizeof(u32) * indices.size()
	};
	auto sVBuffer = MakeOwned<VStageBuffer>(stageVertProps, context.GetDevice());
	auto sIBuffer = MakeOwned<VStageBuffer>(stageIndProps, context.GetDevice());

	DstBufferProps dstVertProps =
	{
		.sizeInBytes = sizeof(Vertex) * triangle.size(),
		.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
	};
	DstBufferProps dstIndProps =
	{
		.sizeInBytes = sizeof(u32) * indices.size(),
		.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT
	};
	auto dstVBuffer = MakeOwned<VDstBuffer>(dstVertProps, context.GetDevice());
	auto dstIBuffer = MakeOwned<VDstBuffer>(dstIndProps, context.GetDevice());

	sVBuffer->MapMemory({ triangle.data(), sizeof(Vertex) * triangle.size() });
	sIBuffer->MapMemory({ indices.data(), sizeof(Vertex) * triangle.size() });

	transferBuffer->BeginRecording(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
	transferBuffer->CopyBufferToBuffer(&*sVBuffer, &*dstVBuffer);
	transferBuffer->CopyBufferToBuffer(&*sIBuffer, &*dstIBuffer);
	transferBuffer->EndRecording();

	context.GetDevice()->SubmitQueue(context.GetTransferQueue(), &*transferBuffer, nullptr, &*testSem, nullptr, VK_PIPELINE_STAGE_VERTEX_SHADER_BIT);

	window.Show();
	while (!window.IsClosed())
	{
		window.PollEvents();
		context.PrepareFrame();
		cmdBuffer->BeginRecording(context.GetRenderPass(), context.GetFramebuffer());
		cmdBuffer->BindPipeline(context.GetPass()->GetPipeline());
		cmdBuffer->BindVertexBuffer(&*dstVBuffer);
		cmdBuffer->BindIndexBuffer(&*dstIBuffer, 0);
		cmdBuffer->DrawIndexed(3, 0, 0, 0, 1);
		cmdBuffer->EndRecording();
		context.Execute(&*cmdBuffer);
		context.SubmitFrame();
	}

	context.GetDevice()->WaitForIdle();

	cmdBuffer->Destroy();
	transferBuffer->Destroy();
	pool->Destroy();
	transferPool->Destroy();
	testSem->Destroy();
	sVBuffer->Destroy();
	sIBuffer->Destroy();
	dstVBuffer->Destroy();
	dstIBuffer->Destroy();

	context.Shutdown();

	return 0;
}