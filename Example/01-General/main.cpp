#include <Engine/Core/Core.h>
#include <Engine/Window/WindowManager.h>

#include <Engine/Vulkan/RHI/Instance/VInstance.h>
#include <Engine/Vulkan/RHI/Device/VDevice.h>
#include <Engine/Vulkan/RHI/Queue/VQueue.h>
#include <Engine/Vulkan/RHI/Swapchain/VSwapchain.h>

using namespace MAGE;

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

	window.Show();
	while (!window.IsClosed())
	{
		window.PollEvents();
	}
	window.Hide();

	window.Destroy();
}
