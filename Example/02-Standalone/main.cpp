#include <Engine/Core/Core.h>
#include <Engine/Window/WindowManager.h>
#include <Engine/Rendering/RenderContext.h>

#include <Engine/VulkanRHI/Command/VCmdPool.h>
#include <Engine/VulkanRHI/Command/VCmdBuffer.h>

#include <Engine/VulkanRHI/Shader/VShader.h>
#include <Engine/VulkanRHI/Pipeline/VPipeline.h>

#include <Engine/VulkanRHI/Buffer/VStageBuffer.h>
#include <Engine/VulkanRHI/Buffer/VDstBuffer.h>
#include <Engine/VulkanRHI/Buffer/VCombinedBuffer.h>
#include <Engine/VulkanRHI/Descriptor/VDescBuffer.h>

#include <Engine/VulkanRHI/Descriptor/VDescPool.h>
#include <Engine/VulkanRHI/Descriptor/VDescSet.h>

using namespace MAGE;

struct Vertex {
	Math::Vec3f Pos;
	Math::Vec3f Normal;
	Math::Vec3f Tangent;
	Math::Vec3f Bitangent;
	Math::Vec4f Color;
	Math::Vec2f UV;
};

Vector<Vertex> triangle = {
	{{ -0.5f, -0.5f,  0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }},
	{{ -0.5f,  0.5f,  0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }},
	{{  0.5f, -0.5f,  0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f }},
	{{  0.5f,  0.5f,  0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f }}
};

struct MVP
{
	Math::Mat4f model;
	Math::Mat4f view;
	Math::Mat4f proj;
};

MVP testmvp = { Math::Mat4f(1.0f), Math::Mat4f(1.0f), Math::Mat4f(1.0f) };

void DoMVP()
{
	testmvp.model = glm::translate(testmvp.model, glm::vec3(0.f, 0.f, 0.f));
	testmvp.model = glm::rotate(testmvp.model, glm::radians(0.f), glm::vec3(1.0f, 0.0f, 0.0f));
	testmvp.model = glm::rotate(testmvp.model, glm::radians(0.f), glm::vec3(0.0f, 1.0f, 0.0f));
	testmvp.model = glm::rotate(testmvp.model, glm::radians(1.f), glm::vec3(0.0f, 0.0f, 1.0f));

	testmvp.proj = glm::perspective(glm::radians(74.0f), 1280.f / 720.f, 0.1f, 100.f);
	testmvp.view = glm::lookAt(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	testmvp.proj[1][1] *= -1;
}

Vector<u32> indices = { 0, 1, 2, 1, 3, 2 };

int main(int argC, char** argV)
{
	DoMVP();

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

	auto descBuffer = context.GetPass()->GetLayout()->CreateDescBuffer(1, VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT);

	CombinedBufferProps cbProp =
	{
		.sizeInBytes = sizeof(MVP),
		.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT
	};
	auto uniformBuffer = MakeOwned<VCombinedBuffer>(cbProp, context.GetDevice());

	uniformBuffer->MapMemory({ &testmvp, sizeof(MVP) });
	descBuffer->MapMemory();
	descBuffer->SetupData(&*uniformBuffer);

	//DescPoolProps poolProp = {};
	//poolProp.maxSets = 1;
	//poolProp.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	//poolProp.poolSizes.push_back({VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1});
	//auto descPool = MakeOwned<VDescPool>(poolProp, context.GetDevice());

	//DescSetProps setProp = {};
	//setProp.layout = context.GetPass()->GetLayout();
	//setProp.pool = &*descPool;
	//auto descSet = MakeOwned<VDescSet>(setProp, context.GetDevice());

	//VkDescriptorBufferInfo bufferInfo = {};
	//bufferInfo.buffer = uniformBuffer->GetBuffer();
	//bufferInfo.offset = 0;
	//bufferInfo.range = sizeof(MVP);

	//VkWriteDescriptorSet writeDescriptorSet = {};
	//writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	//writeDescriptorSet.dstSet = descSet->GetSet();
	//writeDescriptorSet.dstBinding = 0;
	//writeDescriptorSet.dstArrayElement = 0;
	//writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	//writeDescriptorSet.descriptorCount = 1;
	//writeDescriptorSet.pBufferInfo = &bufferInfo;

	//vkUpdateDescriptorSets(context.GetDevice()->GetDevice(), 1, &writeDescriptorSet, 0, nullptr);

	window.Show();
	while (!window.IsClosed())
	{
		DoMVP();
		memcpy(uniformBuffer->GetData(), &testmvp, sizeof(MVP));

		window.PollEvents();
		context.PrepareFrame();

		RecordingProps recProp = {};
		recProp.colorAttachments.push_back(VK_FORMAT_R8G8B8A8_UNORM);
		cmdBuffer->BeginRecording(recProp);
		cmdBuffer->BindPipeline(context.GetPass()->GetPipeline());
		//cmdBuffer->BindDescriptorSet(&*descSet);
		cmdBuffer->BindDescriptorBuffer(&*descBuffer);
		cmdBuffer->BindVertexBuffer(&*dstVBuffer);
		cmdBuffer->BindIndexBuffer(&*dstIBuffer, 0);
		cmdBuffer->DrawIndexed(6, 0, 0, 0, 1);
		cmdBuffer->EndRecording();
		context.Execute(&*cmdBuffer);
		context.SubmitFrame();
	}

	context.GetDevice()->WaitForIdle();

	//descSet->Destroy();
	//descPool->Destroy();
	descBuffer->Destroy();
	uniformBuffer->Destroy();
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