#include "PlatformDirectory.h"

#if defined(MAGE_WINDOWS)
#include "Win32/Win32Directory.h"
typedef MAGE::Win32Directory PlatformDependency;
#endif

#if defined(MAGE_LINUX)
#include "Linux/LinuxDirectory.h"
typedef MAGE::LinuxDirectory PlatformDependency;
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

	b8 PlatformDirectory::GetFiles(const String& path, Vector<String>& files)
	{
		return PlatformDependency::GetFiles(path, files);
	}

	b8 PlatformDirectory::GetDirectories(const String& path, Vector<String>& directories)
	{
		return PlatformDependency::GetDirectories(path, directories);
	}

	b8 PlatformDirectory::GetFilesWithExtension(const String& path, const String& extension, Vector<String>& files)
	{
		return PlatformDependency::GetFilesWithExtension(path, extension, files);
	}
}