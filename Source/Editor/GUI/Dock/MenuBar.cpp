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
				if (ImGui::MenuItem("New Scene"))
				{
				}
				if (ImGui::MenuItem("Open Scene"))
				{
				}
				if (ImGui::MenuItem("Save Scene"))
				{
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Exit"))
				{
					Manager::Window::Get().GetWindow().Quit();
				}
				
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