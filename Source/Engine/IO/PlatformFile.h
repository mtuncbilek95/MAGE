/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Data/Memory/RawBuffer.h"
#include "Engine/Data/Memory/OwnedBuffer.h"

namespace MAGE
{
	namespace PlatformFile
	{
		bool Exists(const String& path);
		bool Create(const String& path, RawBuffer& buffer);
		bool Read(const String& path, OwnedBuffer& buffer, bool nullTerminated = false, const usize start = 0, const usize end = 0);
		bool Read(const String& path, String& buffer, const usize start = 0, const usize end = 0);
		bool Write(const String& path, RawBuffer& buffer, const usize startOffset = 0);
		bool Write(const String& path, const String& buffer, const usize startOffset = 0);
		bool Rename(const String& oldPath, const String& newPath);
		bool Copy(const String& oldPath, const String& newPath);
		bool Move(const String& oldPath, const String& newPath);
		bool Delete(const String& path);
	}
}
