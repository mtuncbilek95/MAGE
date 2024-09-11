#pragma once 

#include "Engine/Core/Core.h"
#include "MemoryMappedFile.h"

namespace MAGE
{
	class ENGINE_API SharedFileHelper
	{
	public:
		SharedFileHelper(const String& filePath, s64 initialSize = 0);
		~SharedFileHelper();

		void* GetData() const { return mMemoryMappedFile->mMemData; }

		void Resize(s64 newSize);
		void Trim(s64 newSize);
		void Write(const String& data);
		void MoveOffset(i64 offset);
		bool CopyTo(void* dest, s64 size, s64 offset = 0);
		bool CloseFile();

	private:
		MemoryMappedFile* mMemoryMappedFile;
		s64 mWritten;
	};
}
