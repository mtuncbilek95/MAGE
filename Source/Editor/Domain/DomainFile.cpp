#include "DomainFile.h"

namespace MAGE
{
	DomainFile::DomainFile(const FileProps& desc, DomainFolder* folder)
	{
		m_name = desc.fileName;
		m_path = desc.filePath;
		m_size = desc.fileSize;
		m_ownerFolder = folder;
	}

	DomainFile::~DomainFile()
	{
	}
}
