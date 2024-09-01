#include "PlatformAPI.h"

#include <spdlog/spdlog.h>

namespace MAGE
{
	void PlatformAPI::InitializeArguments(i32 argc, char** argv)
	{
		String enginePath = argv[0];
		String searchString = "MAGE";

		size_t pos = enginePath.find(searchString);
		if (pos != std::string::npos) {
			size_t lastSlashPos = enginePath.rfind("/\\", pos);
			if (lastSlashPos != std::string::npos)
				mEngineSourcePath = enginePath.substr(0, lastSlashPos + 1);
			else
				mEngineSourcePath = enginePath.substr(0, pos + searchString.length()) + "\\";
		}

		// Change \\ to / for cross-platform compatibility
		std::replace(mEngineSourcePath.begin(), mEngineSourcePath.end(), '\\', '/');

		if (argc > 1)
		{
			mProjectPath = argv[1];
			std::replace(mProjectPath.begin(), mEngineSourcePath.end(), '\\', '/');
		}

		if (!mEngineSourcePath.empty())
			spdlog::warn("Engine source path has been set to: {}", mEngineSourcePath.c_str());

		if (!mProjectPath.empty())
			spdlog::warn("Project path has been set to: {}", mProjectPath.c_str());
		else
			spdlog::error("No project path has been set!");
	}
}