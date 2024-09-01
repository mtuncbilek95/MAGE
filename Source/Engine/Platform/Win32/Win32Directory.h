#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	struct Win32Directory final
	{
		static b8 Exists(const String& path);
		static b8 Create(const String& path);
		static b8 Delete(const String& path);
		static b8 Copy(const String& source, const String& destination);
		static b8 Move(const String& source, const String& destination);
		static b8 Rename(const String& source, const String& destination);
		static Vector<String> GetFiles(const String& path);
		static Vector<String> GetDirectories(const String& path);
		static Vector<String> GetFilesByExtension(const String& path, const String& extension);
	};
}