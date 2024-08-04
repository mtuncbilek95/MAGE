#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Object/API.h>

namespace MAGE
{
	class PlatformAPI : public API<PlatformAPI>
	{
	public:
		PlatformAPI() = default;
		virtual ~PlatformAPI() override = default;

		void InitializeArguments(i32 argc, char** argv)
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
				CORE_LOG(M_WARNING, "Engine source path has been set to: %s", mEngineSourcePath.c_str());

			if (!mProjectPath.empty())
				CORE_LOG(M_WARNING, "Project path has been set to: %s", mProjectPath.c_str());
			else
				CORE_LOG(M_ERROR, "No project path has been set!");
		}

		const String& GetEngineSourcePath() const { return mEngineSourcePath; }
		const String& GetProjectPath() const { return mProjectPath; }

	private:
		String mEngineSourcePath;
		String mProjectPath;
	};
}
