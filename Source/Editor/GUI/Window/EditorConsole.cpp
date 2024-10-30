#include "EditorConsole.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <Engine/ErrorHandler/SystemLog.h>

namespace MAGE
{
	EditorConsole::EditorConsole()
	{
	}

	EditorConsole::~EditorConsole()
	{
	}

	void EditorConsole::Update()
	{
		if (!m_dirty)
		{
			ImGui::SetWindowSize("Console", ImVec2(200, 50));
			m_dirty = true;
		}
	}

	void EditorConsole::Render()
	{
		ImGui::Begin("Console");

		if (ImGui::BeginTable("ConsoleTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX))
		{
			ImGui::TableSetupColumn("Date", ImGuiTableColumnFlags_WidthFixed, ImGui::CalcTextSize("XX:XX:XXXX").x + 20.0f);
			ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthFixed, ImGui::CalcTextSize("XX:XX:XX").x + 20.0f);
			ImGui::TableSetupColumn("Level", ImGuiTableColumnFlags_WidthFixed, ImGui::CalcTextSize("Warning").x + 20.0f);
			ImGui::TableSetupColumn("Message", ImGuiTableColumnFlags_WidthStretch);
			
			ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(10.0f, 5.0f));

			ImGui::TableHeadersRow();

			/*for (const auto& log : SystemLog::GetLogs())
			{
				ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
				ImVec4 restColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
				ImVec4 textColor = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
				String level;
				ImGui::TableNextRow();
				switch (log.level)
				{
				case spdlog::level::trace:
					color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
					level = "Trace";
					break;
				case spdlog::level::debug:
					color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
					level = "Debug";
					break;
				case spdlog::level::info:
					color = ImVec4(0.0f, 100 / 255.f, 100 / 255.f, 1.0f);
					level = "Info";
					break;
				case spdlog::level::warn:
					color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
					level = "Warning";
					break;
				case spdlog::level::err:
					color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
					level = "Error";
					break;
				case spdlog::level::critical:
					color = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);
					level = "Critical";
					break;
				default:
					break;
				}

				ImGui::TableSetColumnIndex(0);
				ImGui::TextColored(textColor, log.date.CharString());
				ImGui::TableSetColumnIndex(1);
				ImGui::TextColored(textColor, log.time.CharString());
				ImGui::TableSetColumnIndex(2);
				ImGui::TextColored(color, level.CharString());
				ImGui::TableSetColumnIndex(3);
				ImGui::TextColored(restColor, log.message.CharString());
			}*/

			ImGui::PopStyleVar();
			ImGui::EndTable();
		}

		ImGui::End();
	}
}