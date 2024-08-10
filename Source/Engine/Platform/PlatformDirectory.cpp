#include "PlatformDirectory.h"

#if defined(MAGE_WINDOWS)
#include <Engine/Win32/Win32Directory.h>
typedef MAGE::Win32Directory PlatformDependency;
#endif

namespace MAGE
{
	b8 PlatformDirectory::Exists(const String& path)
	{
		return PlatformDependency::Exists(path);
	}

	b8 PlatformDirectory::Create(const String& path)
	{
		return PlatformDependency::Create(path);
	}

	b8 PlatformDirectory::Delete(const String& path)
	{
		return PlatformDependency::Delete(path);
	}

	b8 PlatformDirectory::Copy(const String& source, const String& destination)
	{
		return PlatformDependency::Copy(source, destination);
	}

	b8 PlatformDirectory::Move(const String& source, const String& destination)
	{
		return PlatformDependency::Move(source, destination);
	}

	b8 PlatformDirectory::Rename(const String& source, const String& destination)
	{
		return PlatformDependency::Rename(source, destination);
	}

	b8 PlatformDirectory::GetFiles(const String& path, DArray<String>& files)
	{
		return PlatformDependency::GetFiles(path, files);
	}

	b8 PlatformDirectory::GetDirectories(const String& path, DArray<String>& directories)
	{
		return PlatformDependency::GetDirectories(path, directories);
	}

	b8 PlatformDirectory::GetFilesWithExtension(const String& path, const String& extension, DArray<String>& files)
	{
		return PlatformDependency::GetFilesWithExtension(path, extension, files);
	}
}