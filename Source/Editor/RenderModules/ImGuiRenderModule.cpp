#include "ImGuiRenderModule.h"

#include "Editor/API/ImGuiAPI.h"

namespace MAGE
{
	ImGuiRenderModule::ImGuiRenderModule()
	{
		SetModuleName("ImGuiCoreModule");
		mImGuiAPI = ImGuiAPI::GetAPI();
	}

	void ImGuiRenderModule::Start() noexcept
	{
	}

	void ImGuiRenderModule::Update() noexcept
	{
	}

	void ImGuiRenderModule::Stop() noexcept
	{
	}

	b8 ImGuiRenderModule::OnInitialize() noexcept
	{
		auto tempEditor = mImGuiAPI->CreateEditor();
		if (tempEditor == nullptr)
			return false;

		return true;
	}

	void ImGuiRenderModule::OnPreUpdate() noexcept
	{
	}

	void ImGuiRenderModule::OnPostUpdate() noexcept
	{
	}
}
