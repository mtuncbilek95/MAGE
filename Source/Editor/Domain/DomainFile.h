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
	class DomainFolder;

	struct FileProps final
	{
		String fileName;
		String filePath;
		usize fileSize;
	};

	class DomainFile final : public DomainBase
	{
	public:
		DomainFile(const FileProps& desc, DomainFolder* folder);
		~DomainFile() override final;

		DomainFolder* GetOwnerFolder() const { return m_ownerFolder; }

	private:
		DomainFolder* m_ownerFolder;
	};
}