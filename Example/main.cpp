#include <Engine/Core/Core.h>
#include <Engine/Window/WindowManager.h>

#include <Engine/VulkanGraphics/Instance/VulkanInstance.h>
#include <Engine/VulkanGraphics/Device/VulkanDevice.h>
#include <Engine/VulkanGraphics/Queue/VulkanQueue.h>
#include <Engine/VulkanGraphics/Command/VulkanCmdPool.h>
#include <Engine/VulkanGraphics/Command/VulkanCmdBuffer.h>

using namespace MAGE;

int main()
{
	SystemLog::Get().Initialize();
	IndWindowDesc windowProps = {
		.WindowRes = {1280, 720},
		.Mode = WindowMode::Windowed,
		.Title = "TestWindow"
	};
	Manager::Window::Get().InitWindow(windowProps);
	auto& window = Manager::Window::Get().GetWindow();

	InstanceProps instanceProps = {
		.AppName = "TestApp",
		.EngineName = "MAGE",
		.AppVersion = {1, 0, 0},
		.EngineVersion = {1, 0, 0}
	};

	VulkanInstance instance(instanceProps);

	DeviceProps deviceProps = {
		.m_graphicsQueueCount = 1,
		.m_computeQueueCount = 1,
		.m_transferQueueCount = 1
	};
	VulkanDevice device(deviceProps, &instance);

	auto graphicsQueue = device.CreateQueue(VK_QUEUE_GRAPHICS_BIT);

	CmdPoolProps poolProps = {
		.m_queue = graphicsQueue.get(),
		.m_flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
	};

	VulkanCmdPool pool(poolProps, &device);
	auto cmdBuffer = pool.CreateCmdBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

	window.Show();
	while (!window.IsClosed())
	{
		window.PollEvents();
	}

	return 0;
}