#include <Engine/Core/Core.h>
#include <Engine/Window/WindowManager.h>

#include <Engine/Vulkan/RHI/Instance/VInstance.h>
#include <Engine/Vulkan/RHI/Device/VDevice.h>
#include <Engine/Vulkan/RHI/Queue/VQueue.h>
#include <Engine/Vulkan/RHI/Swapchain/VSwapchain.h>
#include <Engine/Vulkan/RHI/Command/VCmdPool.h>
#include <Engine/Vulkan/RHI/Command/VCmdBuffer.h>
#include <Engine/Vulkan/RHI/Sync/VFence.h>
#include <Engine/Vulkan/RHI/Sync/VSemaphore.h>
#include <Engine/Vulkan/RHI/Pipeline/VPipeline.h>
#include <Engine/Vulkan/RHI/Shader/VShader.h>
#include <Engine/Vulkan/RHI/Buffer/VBuffer.h>

using namespace MAGE;

#include <iostream>

struct Vertex
{
	Math::Vec3f pos;
	Math::Vec4f color;
};

Vector<Vertex> square =
{
	{{ -0.5f, -0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }},
	{{ -0.5f,  0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }},
	{{  0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }},
	{{  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }}
};

Vector<u32> indices = { 0, 1, 3, 1, 2, 3 };

int main()
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

	Owned<VInstance> instance = MakeOwned<VInstance>(InstanceProps());
	Owned<VDevice> device = MakeOwned<VDevice>(DeviceProps(), &*instance);

	Owned<VQueue> gQueue = device->CreateQueue(vk::QueueFlagBits::eGraphics);
	Owned<VQueue> cQueue = device->CreateQueue(vk::QueueFlagBits::eCompute);
	Owned<VQueue> tQueue = device->CreateQueue(vk::QueueFlagBits::eTransfer);

	Owned<VSwapchain> swapchain = MakeOwned<VSwapchain>(SwapchainProps(vk::Format::eR8G8B8A8Unorm,
		vk::PresentModeKHR::eFifo, { 1280, 720 }, 2, &*gQueue), &*device);

	CmdPoolProps gPoolProps =
	{
		.queue = &*gQueue,
		.createFlags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer
	};
	Owned<VCmdPool> gPool = MakeOwned<VCmdPool>(gPoolProps, &*device);
	
	Vector<Owned<VCmdBuffer>> gPrimBuffers;
	Vector<Owned<VSemaphore>> renderSemaphores;

	for (u32 i = 0; i < swapchain->GetBufferCount(); i++)
	{
		gPrimBuffers.push_back(gPool->CreateBuffer());
		renderSemaphores.push_back(MakeOwned<VSemaphore>(&*device));
	}

	Owned<VFence> imgFence = MakeOwned<VFence>(&*device);

	ShaderProps shaderProp = {};
	shaderProp.shaderPath = R"(D:\Projects\MAGE\Shaders\Triangle.vert)";
	shaderProp.shaderStage = vk::ShaderStageFlagBits::eVertex;
	Owned<VShader> vShader = MakeOwned<VShader>(shaderProp, &*device);

	shaderProp.shaderPath = R"(D:\Projects\MAGE\Shaders\Triangle.frag)";
	shaderProp.shaderStage = vk::ShaderStageFlagBits::eFragment;
	Owned<VShader> fShader = MakeOwned<VShader>(shaderProp, &*device);

	GraphicsPipelineProps pipeProp = {};
	pipeProp.shaderStages = { &*vShader, &*fShader };
	pipeProp.viewportState.viewport = vk::Viewport(0, 0, 1280, 720, 0.0f, 1.0f);
	pipeProp.viewportState.scissor = vk::Rect2D({ 0,0 }, { 1280, 720 });
	pipeProp.inputAssembler.bindings = { {vk::VertexInputRate::eVertex, {vk::Format::eR32G32B32Sfloat, vk::Format::eR32G32B32A32Sfloat}} };
	pipeProp.dynamicRendering.colorAttachments = { vk::Format::eR8G8B8A8Unorm };
	pipeProp.blendState.attachments = { BlendStateAttachment() };

	Owned<VPipeline> pipeline = MakeOwned<VPipeline>(pipeProp, &*device);

	BufferProps bufferProp = {};
	bufferProp.sizeInBytes = sizeof(Vertex) * square.size();
	bufferProp.usageFlags = vk::BufferUsageFlagBits::eVertexBuffer;
	Owned<VBuffer> vBuffer = MakeOwned<VBuffer>(bufferProp, &*device);
	vBuffer->BindMemory(device->GetAllocator()->GetAvailableMemory(AllocProps(vBuffer->GetTotalSize(), vk::MemoryPropertyFlagBits::eDeviceLocal | vk::MemoryPropertyFlagBits::eHostVisible)));
	vBuffer->Update({ square.data(), bufferProp.sizeInBytes });

	bufferProp.sizeInBytes = indices.size() * sizeof(u32);
	bufferProp.usageFlags = vk::BufferUsageFlagBits::eIndexBuffer;
	Owned<VBuffer> iBuffer = MakeOwned<VBuffer>(bufferProp, &*device);
	iBuffer->BindMemory(device->GetAllocator()->GetAvailableMemory(AllocProps(iBuffer->GetTotalSize(), vk::MemoryPropertyFlagBits::eDeviceLocal | vk::MemoryPropertyFlagBits::eHostVisible)));
	iBuffer->Update({ indices.data(), bufferProp.sizeInBytes });

	window.Show();
	while (!window.IsClosed())
	{
		window.PollEvents();

		u32 imgIndex = swapchain->AcquireNextImage(nullptr, &*imgFence);
		imgFence->Wait();
		imgFence->Reset();
		gPrimBuffers[imgIndex]->BeginRecord(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

		ImageBarrierProps barrier = {};
		barrier.srcPipeline = vk::PipelineStageFlagBits::eColorAttachmentOutput;
		barrier.dstPipeline = vk::PipelineStageFlagBits::eColorAttachmentOutput;
		barrier.srcAccessMask = vk::AccessFlagBits::eColorAttachmentRead;
		barrier.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
		barrier.oldLayout = vk::ImageLayout::ePresentSrcKHR;
		barrier.newLayout = vk::ImageLayout::eColorAttachmentOptimal;
		barrier.image = swapchain->GetSwapchainImage();
		gPrimBuffers[imgIndex]->ImageBarrier(barrier);

		RenderProps renderProp = {};
		renderProp.colorAttachment = { { swapchain->GetSwapchainView(), vk::ImageLayout::eColorAttachmentOptimal, vk::AttachmentLoadOp::eClear,
			vk::AttachmentStoreOp::eStore, vk::ClearColorValue(.39f, .58f, .92f, 1.f), vk::ClearDepthStencilValue(1.f, 0)} };
		renderProp.extent = swapchain->GetImageSize();
		renderProp.layerCount = 1;
		renderProp.viewMask = 0;
		gPrimBuffers[imgIndex]->BeginRendering(renderProp);
		gPrimBuffers[imgIndex]->BindPipeline(&*pipeline);
		gPrimBuffers[imgIndex]->BindVertexBuffer(&*vBuffer);
		gPrimBuffers[imgIndex]->BindIndexBuffer(&*iBuffer);
		gPrimBuffers[imgIndex]->DrawIndexed(indices.size(), 0, 0, 0, 1);
		gPrimBuffers[imgIndex]->EndRendering();

		barrier.srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
		barrier.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead;
		barrier.oldLayout = vk::ImageLayout::eColorAttachmentOptimal;
		barrier.newLayout = vk::ImageLayout::ePresentSrcKHR;
		gPrimBuffers[imgIndex]->ImageBarrier(barrier);
		gPrimBuffers[imgIndex]->EndRecord();

		gQueue->Submit(&*gPrimBuffers[imgIndex], &*renderSemaphores[imgIndex], nullptr, vk::PipelineStageFlagBits::eColorAttachmentOutput, nullptr);

		swapchain->Present(&*renderSemaphores[imgIndex] );
	}
	window.Hide();

	device->WaitForIdle();

	vBuffer->Destroy();
	iBuffer->Destroy();
	pipeline->Destroy();
	vShader->Destroy();
	fShader->Destroy();
	imgFence->Destroy();
	renderSemaphores.clear();
	gPrimBuffers.clear();
	gPool->Destroy();
	swapchain->Destroy();
	device->Destroy();
	instance->Destroy();

	window.Destroy();
}
