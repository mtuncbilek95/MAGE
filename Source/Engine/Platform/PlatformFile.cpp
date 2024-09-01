#include "PlatformFile.h"

#if defined(MAGE_WINDOWS)
#include "Win32/Win32File.h"
typedef MAGE::Win32File Dependency;
#endif

namespace MAGE
{
	b8 PlatformFile::Exists(const String& path)
	{
		return Dependency::Exists(path);
	}

	b8 PlatformFile::Create(const String& path)
	{
		return Dependency::Create(path);
	}

	b8 PlatformFile::Delete(const String& path)
	{
		return Dependency::Delete(path);
	}

	b8 PlatformFile::Copy(const String& source, const String& destination, bool bOverwrite)
	{
		return Dependency::Copy(source, destination, bOverwrite);
	}

	b8 PlatformFile::Move(const String& source, const String& destination)
	{
		return Dependency::Move(source, destination);
	}

	b8 PlatformFile::Rename(const String& source, const String& destination)
	{
		return Dependency::Rename(source, destination);
	}

	b8 PlatformFile::Read(const String& path, String& outFile, bool nullTerminate)
	{
		return Dependency::Read(path, outFile, nullTerminate);
	}

	b8 PlatformFile::Read(const String& path, MemoryOwnedBuffer& outFile, bool nullTerminate)
	{
		return Dependency::Read(path, outFile, nullTerminate);
	}

	b8 PlatformFile::Write(const String& path, const String& data, const u64 offset)
	{
		return Dependency::Write(path, data, offset);
	}

	b8 PlatformFile::Write(const String& path, const MemoryBuffer& data, const u64 offset)
	{
		return Dependency::Write(path, data, offset);
	}

	u64 PlatformFile::GetSize(const String& path)
	{
		return Dependency::GetSize(path);
	}
}