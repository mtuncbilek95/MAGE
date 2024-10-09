#include <Engine/Core/Core.h>
#include <Engine/Window/WindowManager.h>

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

	window.Show();
	while (!window.IsClosed())
	{
		window.PollEvents();
	}

	return 0;
}