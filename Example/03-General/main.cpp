#include <Engine/Core/Core.h>
#include <Engine/Window/WindowManager.h>
#include <Engine/Rendering/RenderContext.h>

#include <Engine/Resources/Texture/Texture2D.h>

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

	auto& context = Gfx::Context::Get();
	context.Init();

	Texture2D test;
	test.CreateImage(R"(D:\Projects\MAGE\Resources\Textures\PurpleGrid.png)");
	test.CreateView();
	test.TransferData();

	while (!window.IsClosed())
	{
		window.PollEvents();
		context.PrepareFrame();
		context.SubmitFrame();
	}
}
