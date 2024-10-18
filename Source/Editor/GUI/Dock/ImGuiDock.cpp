#include "ImGuiDock.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "Editor/Helpers/ImGuiPanelRegistry.h"
#include "Editor/Helpers/ImGuiListener.h"

#include "Editor/GUI/Dock/MenuBar.h"

#include "Editor/GUI/Window/ContentBrowser.h"
#include "Editor/GUI/Window/EntityWindow.h"
#include "Editor/GUI/Window/EntityDetails.h"
#include "Editor/GUI/Window/SceneWindow.h"
#include "Editor/GUI/Window/EditorConsole.h"

namespace MAGE
{
	ImGuiDock::ImGuiDock()
	{
		SetVisibility(true);

		m_menuBar = ImGuiPanelRegistry::Get().RegisterPanel<MenuBar>();
		m_defaultPanels.push_back(ImGuiPanelRegistry::Get().RegisterPanel<EntityWindow>());
		m_defaultPanels.push_back(ImGuiPanelRegistry::Get().RegisterPanel<EntityDetails>());
		m_defaultPanels.push_back(ImGuiPanelRegistry::Get().RegisterPanel<ContentBrowser>());
		m_defaultPanels.push_back(ImGuiPanelRegistry::Get().RegisterPanel<SceneWindow>());
		m_defaultPanels.push_back(ImGuiPanelRegistry::Get().RegisterPanel<EditorConsole>());

		for (auto& panel : m_defaultPanels)
			panel->SetVisibility(true);
	}

	ImGuiDock::~ImGuiDock()
	{
		for (auto& panel : m_defaultPanels)
			panel->SetVisibility(false);
	}

	void ImGuiDock::Update()
	{
		m_menuBar->Update();
	}

	void ImGuiDock::Render()
	{
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::Begin("MainDockSpace", nullptr, windowFlags);
		ImGui::PopStyleVar(3);

		ImGuiID dockspaceID = ImGui::GetID("MainDockSpace");
		
		if (ImGui::DockBuilderGetNode(dockspaceID) == nullptr)
		{
			ImGui::DockBuilderRemoveNode(dockspaceID);
			ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(dockspaceID, viewport->Size);
			// Set dockspace layout
			ImGuiID dockMainID = dockspaceID;
			ImGuiDockNode* nodeMain = ImGui::DockBuilderGetNode(dockMainID);
			if (nodeMain)
			{
				nodeMain->LocalFlags |= ImGuiDockNodeFlags_NoUndocking | ImGuiDockNodeFlags_NoResize;
				ImGui::DockBuilderDockWindow("Scenery", dockMainID);
			}

			ImGuiID dockLeftID = ImGui::DockBuilderSplitNode(dockMainID, ImGuiDir_Left, 0.2f, nullptr, &dockMainID);
			ImGuiDockNode* nodeLeft = ImGui::DockBuilderGetNode(dockLeftID);
			if (nodeLeft)
			{
				nodeLeft->LocalFlags |= ImGuiDockNodeFlags_NoUndocking | ImGuiDockNodeFlags_NoResize;
				ImGui::DockBuilderDockWindow("EntityList", dockLeftID);
			}

			ImGuiID dockRightID = ImGui::DockBuilderSplitNode(dockMainID, ImGuiDir_Right, 0.2f, nullptr, &dockMainID);
			ImGuiDockNode* nodeRight = ImGui::DockBuilderGetNode(dockRightID);
			if (nodeRight)
			{
				nodeRight->LocalFlags |= ImGuiDockNodeFlags_NoUndocking | ImGuiDockNodeFlags_NoResize;
				ImGui::DockBuilderDockWindow("EntityDetails", dockRightID);
			}

			ImGuiID dockBottomID = ImGui::DockBuilderSplitNode(dockMainID, ImGuiDir_Down, 0.2f, nullptr, &dockMainID);
			ImGuiDockNode* nodeBottom = ImGui::DockBuilderGetNode(dockBottomID);
			if (nodeBottom)
			{
				nodeBottom->LocalFlags |= ImGuiDockNodeFlags_NoUndocking | ImGuiDockNodeFlags_NoResize;
				ImGui::DockBuilderDockWindow("ContentBrowser", dockBottomID);
				ImGui::DockBuilderDockWindow("Console", dockBottomID);
			}

			ImGui::DockBuilderFinish(dockspaceID);
		}

		ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
		m_menuBar->Render();

		ImGui::End();
	}
}
