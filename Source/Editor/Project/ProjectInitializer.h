/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Helpers/Singleton.h>

namespace MAGE
{
	class ProjectInitializer final : public Singleton<ProjectInitializer>
	{
	public:
		ProjectInitializer() = default;
		~ProjectInitializer() = default;

		void Initialize(const String& projectPath);
		const String& GetProjectPath() const { return m_projectPath; }
	private:
		String m_projectPath;
		String m_projectName;
	};
}
