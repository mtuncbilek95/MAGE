#include "PlatformFile.h"

#if defined(MAGE_WINDOWS)
#include <Engine/Win32/Win32File.h>
typedef MAGE::Win32File PlatformDependency;
#endif

namespace MAGE
{
	b8 PlatformFile::Exists(const String& path)
	{
		return PlatformDependency::Exists(path);
	}

	b8 PlatformFile::Create(const String& path)
	{
		return PlatformDependency::Create(path);
	}

	b8 PlatformFile::Delete(const String& path)
	{
		return PlatformDependency::Delete(path);
	}

	b8 PlatformFile::Write(const String& path, const String& data, const u64 offset)
	{
		return PlatformDependency::Write(path, data, offset);
	}

	b8 PlatformFile::Write(const String& path, const MemoryBuffer& buffer, const u64 offset)
	{
		return PlatformDependency::Write(path, buffer, offset);
	}

	b8 PlatformFile::Read(const String& path, String& contentOut, const u64 startByte, const u64 endByte)
	{
		return PlatformDependency::Read(path, contentOut, startByte, endByte);
	}

	b8 PlatformFile::Read(const String& path, MemoryOwnedBuffer& view, const u64 startByte, const u64 endByte)
	{
		return PlatformDependency::Read(path, view, startByte, endByte);
	}

	b8 PlatformFile::Copy(const String& source, const String& destination)
	{
		return PlatformDependency::Copy(source, destination);
	}

	b8 PlatformFile::Move(const String& source, const String& destination)
	{
		return PlatformDependency::Move(source, destination);
	}

	b8 PlatformFile::Rename(const String& source, const String& destination)
	{
		return PlatformDependency::Rename(source, destination);
	}

	b8 PlatformFile::GetSize(const String& path, u64& sizeOut)
	{
		return PlatformDependency::GetSize(path, sizeOut);
	}

	b8 PlatformFile::GetName(const String& path, String& nameOut)
	{
		return PlatformDependency::GetName(path, nameOut);
	}

	b8 PlatformFile::GetExtension(const String& path, String& extensionOut)
	{
		return PlatformDependency::GetExtension(path, extensionOut);
	}

	b8 PlatformFile::GetDirectory(const String& path, String& directoryOut)
	{
		return PlatformDependency::GetDirectory(path, directoryOut);
	}
}
