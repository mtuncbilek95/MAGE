#include "RenderContext.h"

#include "Engine/Window/WindowManager.h"
#include "Engine/Resources/ControlUnit/TransferBatch.h"

#if defined(DELUSION_WINDOWS)
VkFormat preFormat = VK_FORMAT_R8G8B8A8_UNORM;
#elif defined(DELUSION_LINUX)
VkFormat preFormat = VK_FORMAT_B8G8R8A8_UNORM;
#endif

namespace MAGE
{
	void Gfx::Context::Init()
	{
		InstanceProps instanceProps =
		{
			.appName = "TestApp",
			.engineName = "MAGE",
			.appVersion = Math::Vec3i(1, 0, 0),
			.engineVersion = Math::Vec3i(1, 0, 0)
		};
		m_instance = MakeOwned<VInstance>(instanceProps);

		DeviceProps deviceProps =
		{
			.graphicsQueueCount = 1,
			.computeQueueCount = 1,
			.transferQueueCount = 1
		};
		m_device = MakeOwned<VDevice>(deviceProps, &*m_instance);

		m_graphicsQueue = m_device->CreateQueue(VK_QUEUE_GRAPHICS_BIT);
		m_computeQueue = m_device->CreateQueue(VK_QUEUE_COMPUTE_BIT);
		m_transferQueue = m_device->CreateQueue(VK_QUEUE_TRANSFER_BIT);

		SwapchainProps swapchainProps =
		{
			.format = preFormat,
			.presentMode = VK_PRESENT_MODE_FIFO_RELAXED_KHR,
			.imageSize = Manager::Window::Get().GetWindow().GetWindowRes(),
			.imageCount = 3,
			.graphicsQueue = &*m_graphicsQueue
		};
		m_swapchain = MakeOwned<VSwapchain>(swapchainProps, &*m_device);

		for (u32 i = 0; i < m_swapchain->GetImageCount(); i++)
			m_renderSemaphores.push_back(m_device->CreateSyncSemaphore());

		m_acquireFence = m_device->CreateSyncFence(false);

		CmdPoolProps cmdPoolProps =
		{
			.queue = &*m_graphicsQueue,
			.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
		};
		m_commandPool = MakeOwned<VCmdPool>(cmdPoolProps, &*m_device);

		for (u32 i = 0; i < m_swapchain->GetImageCount(); i++)
		{
			m_commandBuffers.push_back(m_commandPool->CreateCmdBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY));
		}

		m_testPass = MakeOwned<ForwardPass>();

		TransferBatch::Get().Init(&*m_transferQueue);
	}

	void Gfx::Context::PrepareFrame()
	{
		TransferBatch::Get().ExecuteThemAll();

		m_reqImIndex = m_swapchain->AcquireNextImage(nullptr, &*m_acquireFence);

		m_device->WaitForFence(&*m_acquireFence);
		m_device->ResetFence(&*m_acquireFence);

		m_commandBuffers[m_reqImIndex]->BeginRecording(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
		m_commandBuffers[m_reqImIndex]->BeginRenderPass(m_testPass->GetRenderPass(), m_testPass->GetFramebuffer(m_reqImIndex), VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
	}

	void Gfx::Context::Execute(VCmdBuffer* secBuffer)
	{
		m_commandBuffers[m_reqImIndex]->ExecuteCommand(secBuffer);
	}

	void Gfx::Context::SubmitFrame()
	{
		m_commandBuffers[m_reqImIndex]->EndRenderPass();
		m_commandBuffers[m_reqImIndex]->EndRecording();

		m_device->SubmitQueue(&*m_graphicsQueue, &*m_commandBuffers[m_reqImIndex], nullptr,
			&*m_renderSemaphores[m_reqImIndex], nullptr, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

		m_swapchain->Present(m_reqImIndex, &*m_renderSemaphores[m_reqImIndex]);
	}

	void Gfx::Context::Shutdown()
	{
		m_device->WaitForIdle();

		m_testPass.reset();

		TransferBatch::Get().Destroy();

		for (auto& cmd : m_commandBuffers)
			cmd->Destroy();

		m_commandPool->Destroy();
		m_acquireFence->Destroy();
		
		for (auto& sem : m_renderSemaphores)
			sem->Destroy();

		m_transferQueue->Destroy();
		m_computeQueue->Destroy();
		m_graphicsQueue->Destroy();

		m_swapchain->Destroy();
		m_device->Destroy();
		m_instance->Destroy();
	}
}