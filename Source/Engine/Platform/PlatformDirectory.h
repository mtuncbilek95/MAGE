#pragma once

#include <Engine/Core/Definitions.h>
#include <Engine/Core/StdFix.h>

namespace MAGE
{
	class PlatformDirectory final
	{
	public:
		static b8 Exists(const String& path);
		static b8 Create(const String& path);
		static b8 Delete(const String& path);
		static b8 Copy(const String& source, const String& destination);
		static b8 Move(const String& source, const String& destination);
		static b8 Rename(const String& source, const String& destination);
		static b8 GetFiles(const String& path, Vector<String>& files);
		static b8 GetDirectories(const String& path, Vector<String>& directories);
		static b8 GetFilesWithExtension(const String& path, const String& extension, Vector<String>& files);

	public:
		PlatformDirectory() = delete;
		~PlatformDirectory() = delete;
	};
}