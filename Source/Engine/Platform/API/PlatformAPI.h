#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Object/API.h"

namespace MAGE
{
	class PlatformAPI : public API<PlatformAPI>
	{
	public:
		PlatformAPI() = default;
		virtual ~PlatformAPI() override = default;

		void InitializeArguments(i32 argc, char** argv);

		const Path& GetEngineSourcePath() const { return mEngineSourcePath; }
		const Path& GetProjectPath() const { return mProjectPath; }

	private:
		Path mEngineSourcePath;
		Path mProjectPath;
	};
}
