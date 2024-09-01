#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Memory/MemoryBuffer.h"
#include "Engine/Memory/MemoryOwnedBuffer.h"

namespace MAGE
{
	struct Win32File final
	{
		static b8 Exists(const String& path);
		static b8 Create(const String& path);
		static b8 Delete(const String& path);
		static b8 Copy(const String& source, const String& destination, bool bOverwrite = false);
		static b8 Move(const String& source, const String& destination);
		static b8 Rename(const String& source, const String& destination);
		static b8 Read(const String& path, String& outFile, bool nullTerminate = false);
		static b8 Read(const String& path, MemoryOwnedBuffer& outFile, bool nullTerminate = false);
		static b8 Write(const String& path, const String& data, const u64 offset);
		static b8 Write(const String& path, const MemoryBuffer& data, const u64 offset);
		static u64 GetSize(const String& path);
	};
}