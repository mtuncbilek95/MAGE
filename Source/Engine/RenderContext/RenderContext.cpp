#include "RenderContext.h"

namespace MAGE
{
	void RenderContext::GenerateContext()
	{
		WindowProps windProp =
		{
			.windowTitle = "Hello Triangle",
			.windowSize = {1600, 900},
			.isWindowed = true
		};
		m_window = MakeOwned<IndWindow>(windProp);
		InstanceProps gInsProp =
		{
			.appName = "Hello Triangle",
			.engineName = "MAGE::Delusion",
			.appVersion = {1, 0, 0},
			.engineVersion = {1, 0, 0}
		};
		m_instance = MakeOwned<VInstance>(gInsProp);

		DeviceProps gDevProp =
		{
			.graphicsQueueCount = 1,
			.computeQueueCount = 1,
			.transferQueueCount = 1
		};
		m_device = MakeOwned<VDevice>(gDevProp, &*m_instance);
		m_alloc = MakeOwned<VAllocator>(&*m_device);

		m_gQueue = m_device->CreateQueue(VK_QUEUE_GRAPHICS_BIT);
		m_tQueue = m_device->CreateQueue(VK_QUEUE_TRANSFER_BIT);
		m_cQueue = m_device->CreateQueue(VK_QUEUE_COMPUTE_BIT);

		SwapchainProps swapProp =
		{
			.imageSize = { 1600, 900 },
			.imageCount = 2,
			.format = VK_FORMAT_R8G8B8A8_UNORM,
			.presentMode = VK_PRESENT_MODE_FIFO_KHR,
			.graphicsQueue = &*m_gQueue,
			.usedWindow = &*m_window
		};
		m_swapchain = MakeOwned<VSwapchain>(swapProp, &*m_device);

		CmdPoolProps gPoolProps =
		{
			.queue = &*m_gQueue,
			.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
		};
		m_presentPool = MakeOwned<VCmdPool>(gPoolProps, &*m_device);

		for (u32 i = 0; i < swapProp.imageCount; i++)
		{
			m_imageSemaphores.push_back(MakeOwned<VSemaphore>(&*m_device));
			m_presentSemaphores.push_back(MakeOwned<VSemaphore>(&*m_device));
			m_inflightFences.push_back(MakeOwned<VFence>(false, &*m_device));
			m_presentBuffers.push_back(m_presentPool->CreateCmdBuffer());
		}

		m_window->Show();
	}

	void RenderContext::PrepareFrame()
	{
		m_window->PollEvents();
		
		// TODO: Do transfer work first
		// TODO: Do compute? second

		m_nextIndex = m_swapchain->AcquireNextImage(&*m_imageSemaphores[m_currentIndex], nullptr);
		m_presentBuffers[m_nextIndex]->BeginRecord(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

		ImageBarrierProps barrier = {};
		barrier.srcPipeline = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		barrier.dstPipeline = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		barrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		barrier.image = m_swapchain->GetCurrentImage();
		m_presentBuffers[m_nextIndex]->ImageBarrier(barrier);

		RenderProps renderProp = {};
		renderProp.colorAttachment = { { m_swapchain->GetCurrentView(), VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_ATTACHMENT_LOAD_OP_CLEAR,
			VK_ATTACHMENT_STORE_OP_STORE, {.39f, .58f, .92f, 1.f}, {1.f, 0}} };
		renderProp.extent = { m_swapchain->GetImageSize().x, m_swapchain->GetImageSize().y };
		renderProp.layerCount = 1;
		renderProp.viewMask = 0;
		m_presentBuffers[m_nextIndex]->BeginRendering(renderProp);
	}

	void RenderContext::ExecuteFrame(const vector<VCmdBuffer*>& buffers)
	{
		m_presentBuffers[m_nextIndex]->ExecuteCommands(buffers);
	}

	void RenderContext::SubmitFrame()
	{
		m_presentBuffers[m_nextIndex]->EndRendering();

		ImageBarrierProps barrier = {};
		barrier.srcPipeline = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		barrier.dstPipeline = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		barrier.image = m_swapchain->GetCurrentImage();
		barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
		barrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		m_presentBuffers[m_nextIndex]->ImageBarrier(barrier);
		m_presentBuffers[m_nextIndex]->EndRecord();

		m_gQueue->Submit({ &*m_presentBuffers[m_nextIndex] }, { &*m_imageSemaphores[m_currentIndex] }, { &*m_presentSemaphores[m_nextIndex] }, &*m_inflightFences[m_nextIndex], VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
		m_swapchain->Present({ &*m_presentSemaphores[m_nextIndex] });

		m_inflightFences[m_nextIndex]->Wait();
		m_inflightFences[m_nextIndex]->Reset();

		m_currentIndex = m_nextIndex;
	}
}
