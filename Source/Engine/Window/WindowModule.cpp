#include "WindowModule.h"

#include <Engine/Window/WindowAPI.h>
#include <Engine/Application/Application.h>

#include <nlohmann/json.hpp>
#include <Engine/Platform/PlatformFile.h>
#include <Engine/Platform/API/PlatformAPI.h>

namespace MAGE
{
	// TODO: Its not permanent, will be changed
	WindowDesc GetJson()
	{
		nlohmann::json pJson;
		String path = PlatformAPI::GetAPI()->GetEngineSourcePath() + "Config/EditorConfig.json";
		String jString;
		PlatformFile::Read(path, jString);
		pJson = nlohmann::json::parse(jString);

		WindowDesc windowDesc = {};
		windowDesc.CurrentWindowMode = static_cast<WindowMode>(pJson.at("EditorConfig").at("EditorWindow").at("WindowMode").get<int>());
		windowDesc.WindowSize = Vec2i(100, 100);
		windowDesc.WindowSize = Vec2u(pJson.at("EditorConfig").at("EditorWindow").at("Resolution")[0].get<int>(), pJson.at("EditorConfig").at("EditorWindow").at("Resolution")[1].get<int>());
		windowDesc.WindowTitle = pJson.at("EditorConfig").at("EditorTitle").get<String>() + " - " + pJson.at("EditorConfig").at("EditorVersion").get<String>();

		CORE_LOG(M_INFO, "Currently using: %s", windowDesc.WindowTitle.c_str());

		return windowDesc;
	}

	WindowModule::WindowModule()
	{
		SetModuleName("WindowModule");
		mWindowAPI = WindowAPI::GetAPI();
	}

	WindowModule::~WindowModule()
	{
	}

	void WindowModule::Start() noexcept
	{
		mWindowAPI->GetDefaultWindow()->Show();
	}

	void WindowModule::Update() noexcept
	{
	}

	void WindowModule::Stop() noexcept
	{
		mWindowAPI->GetDefaultWindow()->Hide();
	}

	b8 WindowModule::OnInitialize() noexcept
	{
		auto tempRefWindow = mWindowAPI->InitializeWindow(GetJson());
		if (tempRefWindow.expired())
			return false;

		return true;
	}

	void WindowModule::OnPreUpdate() noexcept
	{
		mWindowAPI->GetDefaultWindow()->PollEvents();

		if (mWindowAPI->GetDefaultWindow()->IsClosed())
		{
			GetOwnerApp()->RequestQuit();
			GetOwnerApp()->QuitReason("Window has been forced to be closed!");
		}
	}

	void WindowModule::OnPostUpdate() noexcept
	{
	}
}
