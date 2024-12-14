/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Definitions/Definitions.h"
#include "Engine/Definitions/StdNames.h"
#include "Engine/Memory/SharedMemory.h"
#include "Engine/Memory/OwnedMemory.h"

namespace Mage
{
	struct Win32File
	{
	private:
		typedef SharedMemory shareBuf;
		typedef OwnedMemory ownBuf;

	public:
		static b8 Exists(const path& dst);
		static b8 Create(const path& dst);
		static b8 Read(const path& dst, ownBuf& buffer, b8 null, usize start = 0, usize end = 0);
		static b8 Read(const path& dst, string& buffer, usize start = 0, usize end = 0);
		static b8 Write(const path& dst, const shareBuf& buffer, usize start = 0);
		static b8 Write(const path& dst, const string& buffer, usize start = 0);
		static b8 Rename(const path& src, const path& dst);
		static b8 Copy(const path& src, const path& dst);
		static b8 Move(const path& src, const path& dst);
		static b8 Delete(const path& dst);
	};
}