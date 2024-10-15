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
#include <Engine/VulkanGraphics/Image/VulkanImage.h>

using namespace MAGE;

int main()
{
	IndWindowDesc windowProps =
	{
		.windowRes = {1920, 1080},
		.mode = WindowMode::Windowed,
		.title = "TestWindow"
	};
	auto window = MakeOwned<IndWindow>(windowProps);

	InstanceProps instanceProps =
	{
		.appName = "TestApp",
		.engineName = "MAGE",
		.appVersion = Math::Vec3i(1, 0, 0),
		.engineVersion = Math::Vec3i(1, 0, 0)
	};
	auto instance = MakeOwned<VulkanInstance>(instanceProps);

	DeviceProps deviceProps =
	{
		.graphicsQueueCount = 1,
		.computeQueueCount = 1,
		.transferQueueCount = 1
	};
	auto device = MakeOwned<VulkanDevice>(deviceProps, &*instance);
	auto grapQueue = device->CreateQueue(VK_QUEUE_GRAPHICS_BIT);

	SwapchainProps swapchainProps =
	{
#if defined(DELUSION_WINDOWS)
		.format = VK_FORMAT_R8G8B8A8_UNORM,
#elif defined(DELUSION_LINUX)
		.format = VK_FORMAT_B8G8R8A8_UNORM,
#endif
		.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR,
		.imageSize = window->GetWindowRes(),
		.imageCount = 2,
		.graphicsQueue = &*grapQueue,
		.windowRef = &*window
	};
	auto swapchain = MakeOwned<VulkanSwapchain>(swapchainProps, &*device);

	CmdPoolProps poolProps =
	{
		.queue = &*grapQueue,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
	};
	auto cmdPool = MakeOwned<VulkanCmdPool>(poolProps, &*device);
	auto cmdBuffer = cmdPool->CreateCmdBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

	auto imageSemaphore = device->CreateSyncSemaphore();
	auto renderSemaphore = device->CreateSyncSemaphore();
	auto fence = device->CreateSyncFence(false);

	window->Show();
	while (!window->IsClosed())
	{
		window->PollEvents();

		u32 imageIndex = swapchain->AcquireNextImage(&*imageSemaphore, nullptr);

		cmdBuffer->BeginRecording(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
		// read to write barrier
		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		barrier.srcQueueFamilyIndex = grapQueue->GetFamilyIndex();
		barrier.dstQueueFamilyIndex = grapQueue->GetFamilyIndex();
		barrier.image = swapchain->GetImage(imageIndex)->GetImage();
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		vkCmdPipelineBarrier(cmdBuffer->GetCmdBuffer(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		cmdBuffer->BeginRenderPass(swapchain->GetImageView(imageIndex), { 1920, 1080 });
		cmdBuffer->EndRenderPass();

		// write to read barrier
		barrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;

		vkCmdPipelineBarrier(cmdBuffer->GetCmdBuffer(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		cmdBuffer->EndRecording();

		device->SubmitQueue(&*grapQueue, &*cmdBuffer, &*imageSemaphore, &*renderSemaphore, &*fence, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

		device->WaitForFence(&*fence);
		device->ResetFence(&*fence);

		swapchain->Present(imageIndex, &*renderSemaphore);
	}

	device->WaitForIdle();

	fence.reset();
	renderSemaphore.reset();
	imageSemaphore.reset();
	cmdBuffer.reset();
	cmdPool.reset();
	swapchain.reset();
	grapQueue.reset();
	device.reset();
	instance.reset();
	window.reset();

	return 0;
}