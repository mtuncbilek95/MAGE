#include "RendererContext.h"

#include "Engine/Window/WindowManager.h"

namespace MAGE
{
	void Gfx::RendererContext::Init()
	{
		InstanceProps instanceProps =
		{
			.appName = "TestApp",
			.engineName = "MAGE",
			.appVersion = Math::Vec3i(1, 0, 0),
			.engineVersion = Math::Vec3i(1, 0, 0)
		};
		m_instance = MakeOwned<VulkanInstance>(instanceProps);

		DeviceProps deviceProps =
		{
			.graphicsQueueCount = 1,
			.computeQueueCount = 1,
			.transferQueueCount = 1
		};
		m_device = MakeOwned<VulkanDevice>(deviceProps, &*m_instance);

		m_graphicsQueue = m_device->CreateQueue(VK_QUEUE_GRAPHICS_BIT);
		m_computeQueue = m_device->CreateQueue(VK_QUEUE_COMPUTE_BIT);
		m_transferQueue = m_device->CreateQueue(VK_QUEUE_TRANSFER_BIT);

		SwapchainProps swapchainProps =
		{
			.format = VK_FORMAT_R8G8B8A8_UNORM,
			.presentMode = VK_PRESENT_MODE_FIFO_KHR,
			.imageSize = Manager::Window::Get().GetWindow().GetWindowRes(),
			.imageCount = 3,
			.graphicsQueue = &*m_graphicsQueue
		};
		m_swapchain = MakeOwned<VulkanSwapchain>(swapchainProps, &*m_device);

		for (u32 i = 0; i < m_swapchain->GetImageCount(); i++)
			m_renderSemaphores.push_back(m_device->CreateSyncSemaphore());

		m_acquireFence = m_device->CreateSyncFence(false);

		CmdPoolProps cmdPoolProps =
		{
			.queue = &*m_graphicsQueue,
			.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
		};
		m_commandPool = MakeOwned<VulkanCmdPool>(cmdPoolProps, &*m_device);

		for (u32 i = 0; i < m_swapchain->GetImageCount(); i++)
		{
			m_commandBuffers.push_back(m_commandPool->CreateCmdBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY));
		}
	}

	void Gfx::RendererContext::PrepareFrame()
	{
		m_reqImIndex = m_swapchain->AcquireNextImage(nullptr, &*m_acquireFence);

		m_device->WaitForFence(&*m_acquireFence);
		m_device->ResetFence(&*m_acquireFence);

		m_commandBuffers[m_reqImIndex]->BeginRecording(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
		// read to write barrier
		VulkanImageBarrier barrier = {};
		barrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		barrier.srcAccess = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
		barrier.dstAccess = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		barrier.srcFamilyIndex = m_graphicsQueue->GetFamilyIndex();
		barrier.dstFamilyIndex = m_graphicsQueue->GetFamilyIndex();
		barrier.image = m_swapchain->GetImage(m_reqImIndex);
		barrier.aspect = VK_IMAGE_ASPECT_COLOR_BIT;
		m_commandBuffers[m_reqImIndex]->SetPipelineImageBarrier(barrier, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
		m_commandBuffers[m_reqImIndex]->BeginRenderPass(m_swapchain->GetImageView(m_reqImIndex), Manager::Window::Get().GetWindow().GetWindowRes());
	}

	void Gfx::RendererContext::SubmitFrame()
	{
		// Execute draw command buffers :)
		m_commandBuffers[m_reqImIndex]->EndRenderPass();

		// write to read barrier
		VulkanImageBarrier barrier = {};
		barrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		barrier.srcAccess = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		barrier.dstAccess = VK_ACCESS_MEMORY_READ_BIT;
		barrier.srcFamilyIndex = m_graphicsQueue->GetFamilyIndex();
		barrier.dstFamilyIndex = m_graphicsQueue->GetFamilyIndex();
		barrier.image = m_swapchain->GetImage(m_reqImIndex);
		barrier.aspect = VK_IMAGE_ASPECT_COLOR_BIT;
		m_commandBuffers[m_reqImIndex]->SetPipelineImageBarrier(barrier, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);

		m_commandBuffers[m_reqImIndex]->EndRecording();

		m_device->SubmitQueue(&*m_graphicsQueue, &*m_commandBuffers[m_reqImIndex], nullptr,
			&*m_renderSemaphores[m_reqImIndex], nullptr, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

		m_swapchain->Present(m_reqImIndex, &*m_renderSemaphores[m_reqImIndex]);
	}

	void Gfx::RendererContext::Shutdown()
	{
		m_device->WaitForIdle();
		m_commandBuffers.clear();
		m_commandPool.reset();
		m_acquireFence.reset();
		m_renderSemaphores.clear();

		m_transferQueue.reset();
		m_computeQueue.reset();
		m_graphicsQueue.reset();

		m_swapchain.reset();
		m_device.reset();
		m_instance.reset();
	}
}