#include <Engine/Core/Core.h>
#include <Engine/Window/WindowManager.h>

#include <Engine/VulkanGraphics/Instance/VulkanInstance.h>
#include <Engine/VulkanGraphics/Device/VulkanDevice.h>
#include <Engine/VulkanGraphics/Queue/VulkanQueue.h>
#include <Engine/VulkanGraphics/Swapchain/VulkanSwapchain.h>
#include <Engine/VulkanGraphics/RenderPass/VulkanRenderPass.h>
#include <Engine/VulkanGraphics/Command/VulkanCmdPool.h>
#include <Engine/VulkanGraphics/Command/VulkanCmdBuffer.h>
#include <Engine/VulkanGraphics/Sync/VulkanSemaphore.h>
#include <Engine/VulkanGraphics/Sync/VulkanFence.h>

using namespace MAGE;

int main()
{
	SystemLog::Get().Initialize();
	IndWindowDesc windowProps =
	{
		.windowRes = {1920, 1080},
		.mode = WindowMode::Windowed,
		.title = "TestWindow"
	};
	Manager::Window::Get().InitWindow(windowProps);
	IndWindow& window = Manager::Window::Get().GetWindow();

	InstanceProps instanceProps =
	{
		.appName = "TestApp",
		.engineName = "MAGE",
		.appVersion = Math::Vec3i(1, 0, 0),
		.engineVersion = Math::Vec3i(1, 0, 0)
	};
	Shared<VulkanInstance> instance = MakeShared<VulkanInstance>(instanceProps);

	DeviceProps deviceProps =
	{
		.graphicsQueueCount = 1,
		.computeQueueCount = 1,
		.transferQueueCount = 1
	};
	Shared<VulkanDevice> device = MakeShared<VulkanDevice>(deviceProps, instance.get());
	Shared<VulkanQueue> grapQueue = device->CreateQueue(VK_QUEUE_GRAPHICS_BIT);

	SwapchainProps swapchainProps =
	{
		.format = VK_FORMAT_R8G8B8A8_UNORM,
		.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR,
		.imageSize = window.GetWindowRes(),
		.imageCount = 3,
		.graphicsQueue = grapQueue.get()
	};
	Shared<VulkanSwapchain> swapchain = MakeShared<VulkanSwapchain>(swapchainProps, device.get());

	CmdPoolProps poolProps =
	{
		.queue = grapQueue.get(),
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
	};
	Shared<VulkanCmdPool> cmdPool = MakeShared<VulkanCmdPool>(poolProps, device.get());
	Shared<VulkanCmdBuffer> cmdBuffer = cmdPool->CreateCmdBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

	Shared<VulkanSemaphore> imageSemaphore = device->CreateSyncSemaphore();
	Shared<VulkanSemaphore> renderSemaphore = device->CreateSyncSemaphore();
	Shared<VulkanFence> fence = device->CreateSyncFence(false);

	window.Show();
	while (!window.IsClosed())
	{
		window.PollEvents();

		u32 imageIndex = swapchain->AcquireNextImage(imageSemaphore.get(), nullptr);

		cmdBuffer->BeginRecording(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
		cmdBuffer->BeginRenderPass(swapchain->GetRenderPass(), window.GetWindowRes(), imageIndex);
		cmdBuffer->EndRenderPass();
		cmdBuffer->EndRecording();

		device->SubmitQueue(grapQueue.get(), cmdBuffer.get(), imageSemaphore.get(), renderSemaphore.get(), fence.get(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
		swapchain->Present(imageIndex, renderSemaphore.get());

		device->WaitForFence(fence.get());
		device->ResetFence(fence.get());
	}
	return 0;
}