/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include <Engine/Core/Core.h>
#include <Engine/ErrorHandler/SystemLog.h>

namespace MAGE
{
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
			SystemLog::AddLog(fmt::to_string(formatted));
		}

		void flush_() override
		{
		}
	};
}
