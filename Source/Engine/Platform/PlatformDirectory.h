#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	struct PlatformDirectory final
	{
		static b8 Exists(const Path& path);
		static b8 Create(const Path& path);
		static b8 Delete(const Path& path);
		static b8 Copy(const Path& source, const Path& destination);
		static b8 Move(const Path& source, const Path& destination);
		static b8 Rename(const Path& source, const Path& destination);
		static Vector<String> GetFiles(const Path& path);
		static Vector<String> GetDirectories(const Path& path);
		static Vector<String> GetFilesByExtension(const Path& path, const String& extension);
	};
}