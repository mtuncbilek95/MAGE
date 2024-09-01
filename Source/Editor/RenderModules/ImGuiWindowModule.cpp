#include "ImGuiWindowModule.h"

#include "Engine/Application/Application.h"
#include "Editor/API/ImGuiWindowAPI.h"
#include "Engine/Graphics/API/GraphicsAPI.h"
#include "Engine/Graphics/Swapchain/Swapchain.h"

#include "Editor/GuiWindow/Built-in/DockWindow.h"
#include "Editor/GuiWindow/Built-in/ContentWindow.h"
#include "Editor/GuiWindow/Built-in/PropertyWindow.h"
#include "Editor/GuiWindow/Built-in/SceneWindow.h"
#include "Editor/GuiWindow/Built-in/WorldWindow.h"

namespace MAGE
{
	ImGuiWindowModule::ImGuiWindowModule()
	{
		SetModuleName("ImGuiWindowModule");
		mImGuiAPI = ImGuiWindowAPI::GetAPI();

		mImGuiAPI->RegisterGuiWindow<DockWindow>();
		mImGuiAPI->RegisterGuiWindow<ContentWindow>();
		mImGuiAPI->RegisterGuiWindow<PropertyWindow>();
		mImGuiAPI->RegisterGuiWindow<SceneWindow>();
		mImGuiAPI->RegisterGuiWindow<WorldWindow>();
	}

	void ImGuiWindowModule::Start() noexcept
	{
		auto windows = mImGuiAPI->GetGuiWindows();

		for (auto& window : windows)
		{
			window->OnInitialize();
		}
	}

	void ImGuiWindowModule::Update() noexcept
	{
		auto swapchain = GraphicsAPI::GetAPI()->GetSwapchain();

		if (swapchain->GetImageSize().x == 0 || swapchain->GetImageSize().y == 0)
			return;

		auto windows = mImGuiAPI->GetGuiWindows();

		for (auto& window : windows)
		{
			window->OnUpdate();
		}

		for (auto& window : windows)
		{
			window->OnPaint();
		}

		if (mImGuiAPI->IsRequestQuit())
		{
			GetOwnerApp()->RequestQuit();
			GetOwnerApp()->QuitReason("Quit from MenuBar.");
		}
	}

	void ImGuiWindowModule::Stop() noexcept
	{
	}

	b8 ImGuiWindowModule::OnInitialize() noexcept
	{
		return true;
	}

	void ImGuiWindowModule::OnPreUpdate() noexcept
	{
	}

	void ImGuiWindowModule::OnPostUpdate() noexcept
	{
	}
}
