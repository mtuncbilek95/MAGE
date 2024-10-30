#include "DomainFolder.h"

namespace MAGE
{
	DomainFolder::DomainFolder(const FolderProps& desc, DomainFolder* owner)
	{
		m_name = desc.fileName;
		m_path = desc.filePath;
		m_size = desc.fileSize;
		m_parent = owner;
	}

	DomainFolder::~DomainFolder()
	{
	}
}
