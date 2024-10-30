#include "ProjectInitializer.h"

namespace MAGE
{
	void ProjectInitializer::Initialize(const String& projectPath)
	{
		// It will be something like this D:/Projects/MyProject/MyProject.mageproj
		// It needs to be split into two parts: D:/Projects/MyProject and MyProject.mageproj
		// The first part will be the project path and the second part will be the project name
	}
}
