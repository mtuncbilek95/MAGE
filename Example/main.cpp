#include <Engine/Core/Core.h>
#include <Engine/IO/PlatformConsole.h>
#include <Engine/Window/WindowManager.h>
#include <Engine/Renderer/RendererContext.h>
#include <Editor/Renderer/ImGuiRenderer.h>

using namespace MAGE;

int main(int argC, char** argV)
{
	SystemLog::Get().InitLogger<ConsoleSink>();

	IndWindowDesc windowProps =
	{
		.windowRes = {1920, 1080},
		.mode = WindowMode::Windowed,
		.title = "TestWindow"
	};
	Manager::Window::Get().InitWindow(windowProps);
	auto& window = Manager::Window::Get().GetWindow();

	auto& context = Gfx::RendererContext::Get();
	context.Init();

	auto uiRenderer = MakeOwned<ImGuiRenderer>();
	uiRenderer->Init();

	window.Show();
	while (!window.IsClosed())
	{
		window.PollEvents();
		context.PrepareFrame();

		uiRenderer->BeginFrame();
		uiRenderer->Render();
		uiRenderer->EndFrame();

		context.SubmitFrame();
	}

	context.GetDevice()->WaitForIdle();
	uiRenderer->Shutdown();
	context.Shutdown();

	return 0;
}