#pragma once

#include <Engine/Core/Definitions.h>
#include <Engine/Core/StdFix.h>

#include <Engine/Memory/MemoryBuffer.h>
#include <Engine/Memory/MemoryOwnedBuffer.h>

namespace MAGE
{
	class PlatformFile final
	{
	public:
		static b8 Exists(const String& path);
		static b8 Create(const String& path);
		static b8 Delete(const String& path);
		static b8 Write(const String& path, const String& data, u64 offset = 0);
		static b8 Write(const String& path, const MemoryBuffer& buffer, u64 offset = 0);
		static b8 Read(const String& path, String& contentOut, u64 startByte = 0, u64 endByte = 0);
		static b8 Read(const String& path, MemoryOwnedBuffer& view, u64 startByte = 0, u64 endByte = 0);
		static b8 Copy(const String& source, const String& destination);
		static b8 Move(const String& source, const String& destination);
		static b8 Rename(const String& source, const String& destination);
		static b8 GetSize(const String& path, u64& sizeOut);
		static b8 GetName(const String& path, String& nameOut);
		static b8 GetExtension(const String& path, String& extensionOut);
		static b8 GetDirectory(const String& path, String& directoryOut);

	public:
		PlatformFile() = delete;
		~PlatformFile() = delete;
	};
}