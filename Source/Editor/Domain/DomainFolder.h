/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include <Engine/Core/Core.h>
#include "../Domain/DomainBase.h"

namespace MAGE
{
	class DomainFile;

	struct FolderProps final
	{
		String fileName;
		String filePath;
		usize fileSize;
	};

	class DomainFolder final : public DomainBase
	{
	public:
		DomainFolder(const FolderProps& desc, DomainFolder* owner = nullptr);
		~DomainFolder();

	private:
		Vector<DomainFolder*> m_folders;
		Vector<DomainFile*> m_files;

		DomainFolder* m_parent = nullptr;
	};
}
