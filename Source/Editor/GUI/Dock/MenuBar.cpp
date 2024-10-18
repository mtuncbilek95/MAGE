#include "MenuBar.h"

#include <imgui.h>

#include "Editor/Helpers/ImGuiPanelRegistry.h"
#include "Editor/Helpers/ImGuiListener.h"

#include <Engine/Window/WindowManager.h>

namespace MAGE
{
	MenuBar::MenuBar()
	{
	}

	MenuBar::~MenuBar()
	{
	}

	void MenuBar::Update()
	{
	}

	void MenuBar::Render()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				ImGui::EndMenu();
			}
		}

		ImGui::EndMenuBar();
	}
}