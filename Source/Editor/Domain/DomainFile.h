/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include <Engine/Core/Core.h>

namespace MAGE
{
	class DomainFolder;

	class DomainFile final
	{
	public:
		DomainFile() = default;
		~DomainFile() = default;

		const String& GetName() const { return m_name; }
		const String& GetPath() const { return m_path; }

		void SetName(const String& name) { m_name = name; }
		void SetPath(const String& path) { m_path = path; }
		
	private:
		String m_name;
		String m_path;

		DomainFolder* m_ownerFolder = nullptr;
	};
}