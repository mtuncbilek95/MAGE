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

namespace MAGE
{
	class ENGINE_API SystemLog final : public Singleton<SystemLog>
	{
	public:
		SystemLog() = default;
		~SystemLog() override = default;

		void Initialize();

	private:
		Shared<spdlog::logger> m_Logger;
	};
}
