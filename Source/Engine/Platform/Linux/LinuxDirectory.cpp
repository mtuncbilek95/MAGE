#include "LinuxDirectory.h"

namespace MAGE
{
    b8 LinuxDirectory::Exists(const String& path)
    {
        return false;
    }

    b8 LinuxDirectory::Create(const String& path)
    {
        return false;
    }

    b8 LinuxDirectory::Delete(const String& path)
    {
        return false;
    }

    b8 LinuxDirectory::Copy(const String& source, const String& destination)
    {
        return false;
    }

    b8 LinuxDirectory::Move(const String& source, const String& destination)
    {
        return false;
    }

    b8 LinuxDirectory::Rename(const String& source, const String& destination)
    {
        return false;
    }

    b8 LinuxDirectory::GetFiles(const String& path, Vector<String>& files)
    {
        return false;
    }

    b8 LinuxDirectory::GetDirectories(const String& path, Vector<String>& directories)
    {
        return false;
    }

    b8 LinuxDirectory::GetFilesWithExtension(const String& path, const String& extension, Vector<String>& files)
    {
        return false;
    }
}