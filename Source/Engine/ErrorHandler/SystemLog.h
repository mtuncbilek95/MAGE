/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Helpers/Singleton.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/sink.h>

namespace MAGE
{
	class SystemLog final : public Singleton<SystemLog>
	{
	public:
		SystemLog() = default;
		~SystemLog() override = default;

		template<typename T = void, typename = std::enable_if_t<std::is_void_v<T> || std::is_base_of_v<spdlog::sinks::sink, T>>>
		auto InitLogger() -> typename std::conditional<std::is_void<T>::value, void, T*>::type
		{
			if (!firstInit)
			{
				// Create console sink
				auto consoleSink = MakeShared<spdlog::sinks::stdout_color_sink_mt>();
				consoleSink->set_level(spdlog::level::trace);
				m_sinks.push_back(consoleSink);

				auto fileSink = MakeShared<spdlog::sinks::basic_file_sink_mt>("../Logs/Output.log", true); // TODO: Change path
				fileSink->set_level(spdlog::level::trace);
				m_sinks.push_back(fileSink);

				firstInit = true;
			}

			// Add template sink to the vector
			Shared<T> customSink;
			if constexpr (!std::is_void_v<T>)
			{
				customSink = MakeShared<T>();
				m_sinks.push_back(MakeShared<T>());
			}

			m_logger = MakeShared<spdlog::logger>("SystemLog", m_sinks.begin(), m_sinks.end());

			// Set log level to trace to log all levels
			m_logger->set_level(spdlog::level::trace);
			m_logger->flush_on(spdlog::level::err);  // Flush logs on error

			spdlog::set_default_logger(m_logger);

			// [Date][Time][Level]: Message
			spdlog::set_pattern("[%m-%d-%Y][%H:%M:%S][%^%l%$]: %v");

			if constexpr (std::is_void_v<T>)
				return;
			else
				return customSink.get();
		}

		static void AddLog(const String& log)
		{
			Get().m_items.push_back(log);
		}

		static Vector<String>& GetLogs()
		{
			return Get().m_items;
		}

		static void ClearLogs()
		{
			Get().m_items.clear();
		}

	private:
		Shared<spdlog::logger> m_logger;
		Vector<spdlog::sink_ptr> m_sinks;
		bool firstInit = false;

		Vector<String> m_items;
	};
}
