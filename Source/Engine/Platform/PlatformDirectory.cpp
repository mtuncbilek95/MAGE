#include "PlatformDirectory.h"

#if defined(MAGE_WINDOWS)
#include "Engine/Win32/Win32Directory.h"
typedef MAGE::Win32Directory Dependency;
#endif

namespace MAGE
{
	b8 PlatformDirectory::Exists(const String& path)
	{
		return Dependency::Exists(path);
	}

	b8 PlatformDirectory::Create(const String& path)
	{
		return Dependency::Create(path);
	}

	b8 PlatformDirectory::Delete(const String& path)
	{
		return Dependency::Delete(path);
	}

	b8 PlatformDirectory::Copy(const String& source, const String& destination)
	{
		return Dependency::Copy(source, destination);
	}

	b8 PlatformDirectory::Move(const String& source, const String& destination)
	{
		return Dependency::Move(source, destination);
	}

	b8 PlatformDirectory::Rename(const String& source, const String& destination)
	{
		return Dependency::Rename(source, destination);
	}

	Vector<String> PlatformDirectory::GetFiles(const String& path)
	{
		return Dependency::GetFiles(path);
	}

	Vector<String> PlatformDirectory::GetDirectories(const String& path)
	{
		return Dependency::GetDirectories(path);
	}

	Vector<String> PlatformDirectory::GetFilesByExtension(const String& path, const String& extension)
	{
		return Dependency::GetFilesByExtension(path, extension);
	}
}