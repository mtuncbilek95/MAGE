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
	class DomainFile;

	class DomainFolder final
	{
	public:
		DomainFolder() = default;
		~DomainFolder() = default;

		void AddFolder(DomainFolder* folder);
		void AddFile(DomainFile* file);

		void RemoveFolder(DomainFolder* folder);
		void RemoveFile(DomainFile* file);

		const String& GetName() const { return m_name; }
		const String& GetPath() const { return m_path; }

		const Vector<DomainFolder*>& GetFolders() const { return m_folders; }
		const Vector<DomainFile*>& GetFiles() const { return m_files; }

		DomainFolder* GetParent() const { return m_parent; }

		void SetName(const String& name) { m_name = name; }
		void SetPath(const String& path) { m_path = path; }

	private:
		String m_name;
		String m_path;

		Vector<DomainFolder*> m_folders;
		Vector<DomainFile*> m_files;

		DomainFolder* m_parent = nullptr;
	};
}
