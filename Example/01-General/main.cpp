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
	{{ -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }},
	{{  0.5f,  0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }},
	{{  0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }}
};

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
		vk::PresentModeKHR::eFifoRelaxed, { 1280, 720 }, 2, &*gQueue), &*device);

	CmdPoolProps gPoolProps =
	{
		.queue = &*gQueue,
		.createFlags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer
	};
	Owned<VCmdPool> gPool = MakeOwned<VCmdPool>(gPoolProps, &*device);
	Owned<VCmdBuffer> gPrimBuffer = gPool->CreateBuffer();

	Owned<VFence> imgFence = MakeOwned<VFence>(&*device);
	Owned<VSemaphore> presentSem = MakeOwned<VSemaphore>(&*device);

	window.Show();
	while (!window.IsClosed())
	{
		window.PollEvents();

		u32 imgIndex = swapchain->AcquireNextImage(nullptr, &*imgFence);
		imgFence->Wait();
		imgFence->Reset();

		gPrimBuffer->BeginRecord(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

		// Barrier swapchain
		ImageBarrierProps barrier = {};
		barrier.srcPipeline = vk::PipelineStageFlagBits::eColorAttachmentOutput;
		barrier.dstPipeline = vk::PipelineStageFlagBits::eColorAttachmentOutput;
		barrier.srcAccessMask = {};
		barrier.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
		barrier.oldLayout = vk::ImageLayout::ePresentSrcKHR;
		barrier.newLayout = vk::ImageLayout::eColorAttachmentOptimal;
		barrier.image = swapchain->GetSwapchainImage();

		gPrimBuffer->ImageBarrier(barrier);

		RenderProps renderProp = {};
		renderProp.colorAttachment = { {swapchain->GetSwapchainView(), vk::ImageLayout::eColorAttachmentOptimal, vk::AttachmentLoadOp::eClear,
			vk::AttachmentStoreOp::eStore, vk::ClearColorValue(.39f, .58f, .92f, 1.f), vk::ClearDepthStencilValue(1.f, 0)} };
		renderProp.extent = swapchain->GetImageSize();
		renderProp.layerCount = 1;
		renderProp.viewMask = 0;
		renderProp.renderFlags = vk::RenderingFlagBits::eContentsSecondaryCommandBuffers;
		gPrimBuffer->BeginRendering(renderProp);
		gPrimBuffer->EndRendering();

		// Barrier again
		barrier.srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
		barrier.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead;
		barrier.oldLayout = vk::ImageLayout::eColorAttachmentOptimal;
		barrier.newLayout = vk::ImageLayout::ePresentSrcKHR;
		gPrimBuffer->ImageBarrier(barrier);
		gPrimBuffer->EndRecord();

		gQueue->Submit({ &*gPrimBuffer }, { &*presentSem }, {}, { vk::PipelineStageFlagBits::eColorAttachmentOutput }, nullptr);
		swapchain->Present({ &*presentSem });
	}
	window.Hide();

	device->WaitForIdle();

	imgFence->Destroy();
	presentSem->Destroy();
	gPrimBuffer->Destroy();
	gPool->Destroy();
	swapchain->Destroy();
	device->Destroy();
	instance->Destroy();

	window.Destroy();
}
