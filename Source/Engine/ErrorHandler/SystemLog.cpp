#include "SystemLog.h"

#include <iostream>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace MAGE
{
	void SystemLog::Initialize()
	{
		try
		{
			// Create a color console sink
			auto console_sink = MakeShared<spdlog::sinks::stdout_color_sink_mt>();
			console_sink->set_level(spdlog::level::trace);

			// Create a file sink
			auto file_sink = MakeShared<spdlog::sinks::basic_file_sink_mt>("Logs/Output.log", true);
			file_sink->set_level(spdlog::level::trace);

			// Combine both sinks
			std::vector<spdlog::sink_ptr> sinks{ console_sink, file_sink };
			m_Logger = MakeShared<spdlog::logger>("SystemLog", sinks.begin(), sinks.end());

			// Set log level to trace to log all levels
			m_Logger->set_level(spdlog::level::trace);
			m_Logger->flush_on(spdlog::level::err);  // Flush logs on error

			spdlog::set_default_logger(m_Logger);

			// Change format of the logger

			// [Date][Time][Level]: Message
			spdlog::set_pattern("[%m-%d-%Y][%H:%M:%S][%^%l%$]: %v");
		}
		catch (const spdlog::spdlog_ex& ex)
		{
			std::cerr << "Logger initialization failed: " << ex.what() << std::endl;
		}
	}
}