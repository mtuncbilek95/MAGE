#include "ConfigCreator.h"

#include "Engine/Platform/PlatformFile.h"

#include <nlohmann/json.hpp>

namespace MAGE
{
	ConfigCreator configFile = ConfigCreator();

	void Config::InitializeConfig(int argC, char** argV)
	{
		configFile.SetConfig(argC, argV);
	}

	Configuration Config::GetAppConfig()
	{
		return configFile.GetConfig();
	}

	void ConfigCreator::SetConfig(int argC, char** argV)
	{
		path ctx(argV[0]);

		for (const auto& part : ctx) {
			m_props.enginePath /= part;
			if (part == "MAGE")
				break;
		}

		if (argC > 1)
		{
			ctx = argV[1];
			string jsonStr;
			PlatformFile::Read(ctx.string(), jsonStr);

			nlohmann::json j = nlohmann::json::parse(jsonStr);
			m_props.projectName = j["ProjectDetails"]["ProjectName"];
			m_props.projectVersion = { j["ProjectDetails"]["ProjectVersion"][0], j["ProjectDetails"]["ProjectVersion"][1], j["ProjectDetails"]["ProjectVersion"][2] };

			m_props.projectPath = ctx.parent_path();
		}
	}
}


