#include <Engine/Core/Core.h>
#include <Engine/Window/WindowManager.h>
#include <Engine/Renderer/RendererContext.h>
#include <Editor/Renderer/ImGuiRenderer.h>

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

		ImGui::ShowDemoWindow();

		uiRenderer->EndFrame();
		context.SubmitFrame();
	}

	return 0;
}