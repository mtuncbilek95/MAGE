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

#include <Engine/VulkanGraphics/Descriptor/VulkanDescLayout.h>
#include <Engine/VulkanGraphics/Descriptor/VulkanDescBuffer.h>

using namespace MAGE;

int main()
{
	IndWindowDesc windowProps =
	{
		.windowRes = {1920, 1080},
		.mode = WindowMode::Windowed,
		.title = "TestWindow"
	};
	Manager::Window::Get().InitWindow(windowProps);
	auto& window = Manager::Window::Get().GetWindow();

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
		.imageSize = window.GetWindowRes(),
		.imageCount = 3,
		.graphicsQueue = &*grapQueue,
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

	// DescriptorLayout
	DescLayoutProps descLayoutProps = {};
	descLayoutProps.bindings =
	{
		{ 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT},
		{ 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT},
		{ 2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT}
	};
	descLayoutProps.initFlags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_DESCRIPTOR_BUFFER_BIT_EXT;
	auto descLayout = MakeOwned<VulkanDescLayout>(descLayoutProps, &*device);
	auto descBuffer = MakeOwned<VulkanDescBuffer>(&*descLayout, &*device);

	window.Show();
	while (!window.IsClosed())
	{
		window.PollEvents();

		u32 imageIndex = swapchain->AcquireNextImage(&*imageSemaphore, nullptr);

		cmdBuffer->BeginRecording(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
		// read to write barrier
		VulkanImageBarrier barrier = {};
		barrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		barrier.srcAccess = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
		barrier.dstAccess = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		barrier.srcFamilyIndex = grapQueue->GetFamilyIndex();
		barrier.dstFamilyIndex = grapQueue->GetFamilyIndex();
		barrier.image = swapchain->GetImage(imageIndex);
		barrier.aspect = VK_IMAGE_ASPECT_COLOR_BIT;
		cmdBuffer->SetPipelineImageBarrier(barrier, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

		cmdBuffer->BeginRenderPass(swapchain->GetImageView(imageIndex), window.GetWindowRes());
		cmdBuffer->EndRenderPass();

		// write to read barrier
		barrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		barrier.srcAccess = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		barrier.dstAccess = VK_ACCESS_MEMORY_READ_BIT;
		cmdBuffer->SetPipelineImageBarrier(barrier, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);

		cmdBuffer->EndRecording();
		device->SubmitQueue(&*grapQueue, &*cmdBuffer, &*imageSemaphore, &*renderSemaphore, &*fence, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

		device->WaitForFence(&*fence);
		device->ResetFence(&*fence);

		swapchain->Present(imageIndex, &*renderSemaphore);
	}

	device->WaitForIdle();

	descBuffer.reset();
	descLayout.reset();
	fence.reset();
	renderSemaphore.reset();
	imageSemaphore.reset();
	cmdBuffer.reset();
	cmdPool.reset();
	swapchain.reset();
	grapQueue.reset();
	device.reset();
	instance.reset();

	return 0;
}