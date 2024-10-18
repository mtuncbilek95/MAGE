#include "ContentBrowser.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace MAGE
{
	ContentBrowser::ContentBrowser()
	{
	}

	ContentBrowser::~ContentBrowser()
	{
	}

	void ContentBrowser::Update()
	{
		if (!m_dirty)
		{
			ImGui::SetWindowSize("ContentBrowser", ImVec2(200, 50));
			m_dirty = true;
		}
	}

	void ContentBrowser::Render()
	{
		ImGui::Begin("ContentBrowser");

		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Create Folder"))
			{
			}
			if (ImGui::MenuItem("Import File"))
			{
			}
			if (ImGui::MenuItem("Create Scene"))
			{
			}
			ImGui::Separator();

			if (ImGui::MenuItem("Create Script"))
			{
			}

			ImGui::Separator();
			if (ImGui::MenuItem("Create Material"))
			{
			}
			if (ImGui::MenuItem("Create Shader"))
			{
			}
			ImGui::EndPopup();
		}
		ImGui::End();
	}
}