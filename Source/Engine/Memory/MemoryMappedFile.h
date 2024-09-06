#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	class MemoryMappedFile
	{
	public:
		MemoryMappedFile(const String& filePath) {}
		~MemoryMappedFile() {}

		void* GetData() const { return mMemData; }
		u64 GetFileSize() const { return mFileSize; }

	private:
		void* mMemData;
		u64 mFileSize;
	};
}
