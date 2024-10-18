#include "ProjectInitializer.h"

namespace MAGE
{
	void ProjectInitializer::Initialize(const String& projectPath)
	{
		// It will be something like this D:/Projects/MyProject/MyProject.mageproj
		// It needs to be split into two parts: D:/Projects/MyProject and MyProject.mageproj
		// The first part will be the project path and the second part will be the project name
		usize index = projectPath.rfind('/');
		m_projectPath = projectPath.substr(0, index + 1);
		m_projectName = projectPath.substr(index + 1, projectPath.size() - index - 1);

		printf("Project Path: %s\n", m_projectPath.c_str());
	}
}
