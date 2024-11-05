/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Objects/Singleton.h"

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
			// Create console sink
			auto consoleSink = MakeShared<spdlog::sinks::stdout_color_sink_mt>();
			consoleSink->set_level(spdlog::level::debug);
			m_sinks.push_back(consoleSink);

			auto fileSink = MakeShared<spdlog::sinks::basic_file_sink_mt>("../Logs/Output.log", true); // TODO: Change path
			fileSink->set_level(spdlog::level::debug);
			m_sinks.push_back(fileSink);

			// Add template sink to the vector
			Shared<T> customSink;
			if constexpr (!std::is_void_v<T>)
			{
				customSink = MakeShared<T>();
				m_sinks.push_back(MakeShared<T>());
			}

			m_logger = MakeShared<spdlog::logger>("SystemLog", m_sinks.begin(), m_sinks.end());

			// m_logger->sinks().push_back() TODO: USE THIS FUCKER
			
			// Set log level to trace to log all levels
			m_logger->set_level(spdlog::level::debug);
			m_logger->flush_on(spdlog::level::err);  // Flush logs on error

			spdlog::set_default_logger(m_logger);

			// [Date][Time][Level]: Message
			spdlog::set_pattern("[%m-%d-%Y][%H:%M:%S][%^%l%$]: %v");

			if constexpr (std::is_void_v<T>)
				return;
			else
				return customSink.get();
		}

	private:
		Shared<spdlog::logger> m_logger;
		Vector<spdlog::sink_ptr> m_sinks;
		//Vector<SplitLog> m_items;
	};

	class ConsoleSink : public spdlog::sinks::base_sink<std::mutex>
	{
	public:
		ConsoleSink() = default;
		~ConsoleSink() = default;

	protected:
		void sink_it_(const spdlog::details::log_msg& msg) override
		{
			spdlog::memory_buf_t formatted;
			this->formatter_->format(msg, formatted);

			/*SplitLog splitter = {};
			String log = fmt::to_string(formatted);
			splitter.date = log.SubString(1, 10);
			splitter.time = log.SubString(13, 8);
			splitter.level = msg.level;
			splitter.message = String(msg.payload.data(), msg.payload.size());

			SystemLog::AddLog(splitter);*/
		}

		void flush_() override
		{
		}
	};
}
