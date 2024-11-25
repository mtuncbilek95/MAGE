/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/StdFix.h"
#include "Engine/Core/GlmFix.h"
#include "Engine/Core/Definitions.h"

namespace MAGE
{
	struct Configuration final
	{
		string projectName;
		Math::Vec3u projectVersion;
		path enginePath = "";
		path projectPath = "";
	};

	class ConfigCreator final
	{
	public:
		ConfigCreator() = default;
		~ConfigCreator() = default;

		void SetConfig(int argC, char** argV);

		const Configuration& GetConfig() const { return m_props; }

	private:
		Configuration m_props;
	};

	namespace Config
	{
		void InitializeConfig(int argC, char** argV);
		Configuration GetAppConfig();
	}
}
