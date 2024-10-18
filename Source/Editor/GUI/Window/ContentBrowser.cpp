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
				spdlog::warn("Create Folder");
			}
			if (ImGui::MenuItem("Import File"))
			{
				spdlog::warn("Import File");
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Create Script"))
			{
				spdlog::warn("Create Script");
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Create Material"))
			{
				spdlog::warn("Create Material");
			}
			if (ImGui::MenuItem("Create Shader"))
			{
				spdlog::warn("Create Shader");
			}
			ImGui::EndPopup();
		}
		ImGui::End();
	}
}