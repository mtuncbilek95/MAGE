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

		const String& GetEngineSourcePath() const { return mEngineSourcePath; }
		const String& GetProjectPath() const { return mProjectPath; }

	private:
		String mEngineSourcePath;
		String mProjectPath;
	};
}
