#include <Engine/Core/Core.h>
#include <Engine/Window/WindowManager.h>
 
#include <Engine/VulkanGraphics/Instance/VulkanInstance.h>

using namespace MAGE;

int main()
{
	SystemLog::Get().Initialize();
	IndWindowDesc windowDesc = {
		.WindowRes = {1280, 720},
		.Mode = WindowMode::Windowed,
		.Title = "TestWindow"
	};
	Manager::Window::Get().InitWindow(windowDesc);
	auto& window = Manager::Window::Get().GetWindow();

	InstanceProps instanceProps = {
		.AppName = "TestApp",
		.EngineName = "MAGE",
		.AppVersion = {1, 0, 0},
		.EngineVersion = {1, 0, 0}
	};

	VulkanInstance instance(instanceProps);

	window.Show();
	while (!window.IsClosed())
	{
		window.PollEvents();
	}

	return 0;
}