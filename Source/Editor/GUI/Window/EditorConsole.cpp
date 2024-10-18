#include "EditorConsole.h"

#include <imgui.h>
#include <imgui_internal.h>

#include <Engine/ErrorHandler/SystemLog.h>
#include <Engine/IO/PlatformConsole.h>

#include "Editor/Core/ConsoleSink.h"

namespace MAGE
{

	struct LogSplit
	{
		String Date;
		String Time;
		String Level;
		String Message;
	};

	LogSplit SplitLog(const String& log)
	{
		LogSplit split;
		size_t first = log.find_first_of('[');
		size_t last = log.find_first_of(']');
		split.Date = log.substr(first, last - first + 1);

		first = log.find_first_of('[', last);
		last = log.find_first_of(']', first);
		split.Time = log.substr(first, last - first + 1);

		first = log.find_first_of('[', last);
		last = log.find_first_of(']', first);
		split.Level = log.substr(first, last - first + 1);

		split.Message = log.substr(last + 1, log.size() - last);
		return split;
	}

	EditorConsole::EditorConsole()
	{
		m_sink = SystemLog::Get().InitLogger<ConsoleSink>();
		//PlatformConsole::HideConsole();
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

		if (ImGui::BeginTable("ConsoleTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX))
		{
			ImGui::TableSetupColumn("Date");
			ImGui::TableSetupColumn("Time");
			ImGui::TableSetupColumn("Level");
			ImGui::TableSetupColumn("Message");
			ImGui::TableHeadersRow();

			for (const auto& log : SystemLog::GetLogs())
			{
				auto split = SplitLog(log);
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::TextUnformatted(split.Date.c_str());
				ImGui::TableSetColumnIndex(1);
				ImGui::TextUnformatted(split.Time.c_str());
				ImGui::TableSetColumnIndex(2);
				ImGui::TextUnformatted(split.Level.c_str());
				ImGui::TableSetColumnIndex(3);
				ImGui::TextUnformatted(split.Message.c_str());
			}

			ImGui::EndTable();
		}

		ImGui::End();
	}
}