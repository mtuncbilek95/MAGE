#include <Engine/Core/Core.h>
#include <Engine/Window/WindowManager.h>
#include <Engine/Renderer/RendererContext.h>
#include <Editor/Renderer/ImGuiRenderer.h>
#include <Editor/Project/ProjectInitializer.h>


using namespace MAGE;

int main(int argC, char** argV)
{
	ProjectInitializer::Get().Initialize("D:/Projects/MyProject/MyProject.mageproj");

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
		
		// Regular rendering calls in here
		/* Do something*/

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