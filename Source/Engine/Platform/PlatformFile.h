/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Memory/RawBuffer.h"
#include "Engine/Memory/OwnedBuffer.h"

namespace MAGE
{
	namespace PlatformFile
	{
		bool Exists(const string& path);
		bool Create(const string& path, const RawBuffer& buffer);
		bool Read(const string& path, OwnedBuffer& buffer, bool nullTerminated = false, const usize start = 0, const usize end = 0);
		bool Read(const string& path, string& buffer, const usize start = 0, const usize end = 0);
		bool Write(const string& path, const RawBuffer& buffer, const usize startOffset = 0);
		bool Write(const string& path, const string& buffer, const usize startOffset = 0);
		bool Rename(const string& oldPath, const string& newPath);
		bool Copy(const string& oldPath, const string& newPath);
		bool Move(const string& oldPath, const string& newPath);
		bool Delete(const string& path);
	}
}
