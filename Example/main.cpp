#include <Engine/Core/Core.h>
#include <Engine/Window/WindowManager.h>

#include <Engine/VulkanGraphics/Instance/VulkanInstance.h>
#include <Engine/VulkanGraphics/Device/VulkanDevice.h>
#include <Engine/VulkanGraphics/Queue/VulkanQueue.h>
#include <Engine/VulkanGraphics/Swapchain/VulkanSwapchain.h>
#include <Engine/VulkanGraphics/Image/VulkanImage.h>
#include <Engine/VulkanGraphics/Image/VulkanImageView.h>

using namespace MAGE;

int main()
{
	SystemLog::Get().Initialize();
	IndWindowDesc windowProps = 
	{
		.WindowRes = {1280, 720},
		.Mode = WindowMode::Windowed,
		.Title = "TestWindow"
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
	VulkanInstance instance = VulkanInstance(instanceProps);

	DeviceProps deviceProps = 
	{
		.m_graphicsQueueCount = 1,
		.m_computeQueueCount = 1,
		.m_transferQueueCount = 1
	};
	VulkanDevice device = VulkanDevice(deviceProps, &instance);
	VulkanQueue grapQueue = device.CreateQueue(VK_QUEUE_GRAPHICS_BIT);

	SwapchainProps swapchainProps = 
	{
		.format = VK_FORMAT_R8G8B8A8_UNORM,
		.presentMode = VK_PRESENT_MODE_FIFO_KHR,
		.imageSize = window.GetWindowRes(),
		.imageCount = 3,
		.graphicsQueue = &grapQueue
	};
	VulkanSwapchain swapchain = VulkanSwapchain(swapchainProps, &device);

	ImageProps imageProps =
	{
		.imageSize = {1280, 720, 1},
		.mipLevels = 1,
		.arrayLayers = 1,
		.imageType = VK_IMAGE_TYPE_2D,
		.format = VK_FORMAT_R8G8B8A8_UNORM,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT
	};
	VulkanImage image = VulkanImage(imageProps, &device);

	ImageViewProps viewProps =
	{
		.viewType = VK_IMAGE_VIEW_TYPE_2D,
		.aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT,
		.baseMipLevel = 0,
		.baseArrayLayer = 0
	};
	VulkanImageView imageView = image.CreateView(viewProps);

	window.Show();
	while (!window.IsClosed())
	{
		window.PollEvents();
	}

	return 0;
}