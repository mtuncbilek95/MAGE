#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Memory/MemoryBuffer.h"
#include "Engine/Memory/MemoryOwnedBuffer.h"

namespace MAGE
{
	struct PlatformFile final
	{
		static b8 Exists(const Path& path);
		static b8 Create(const Path& path);
		static b8 Delete(const Path& path);
		static b8 Copy(const Path& source, const Path& destination, bool bOverwrite = false);
		static b8 Move(const Path& source, const Path& destination);
		static b8 Rename(const Path& source, const Path& destination);
		static b8 Read(const Path& path, String& outFile, bool nullTerminate = false);
		static b8 Read(const Path& path, MemoryOwnedBuffer& outFile, bool nullTerminate = false);
		static b8 Write(const Path& path, const String& data, const u64 offset = 0);
		static b8 Write(const Path& path, const MemoryBuffer& data, const u64 offset = 0);
		static u64 GetSize(const Path& path);
	};
}