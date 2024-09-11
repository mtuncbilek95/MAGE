#pragma once

#include "Engine/Core/Core.h"

#if defined(MAGE_WINDOWS)
#include <Windows.h>
#endif

namespace MAGE
{
	class ENGINE_API MemoryMappedFile
	{
		friend class SharedFileHelper;
	public:
		MemoryMappedFile(const String& filePath);
		~MemoryMappedFile();

		void* GetData() const { return mMemData; }
		u64 GetFileSize() const { return mFileSize; }

		bool CloseFile();

	private:
		void* mMemData;
		u64 mFileSize;

#if defined(MAGE_WINDOWS)
		HANDLE mFileHandle;
		HANDLE mFileMapping;
#endif
	};
}
