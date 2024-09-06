#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Object/IObject.h"

namespace MAGE
{
	struct GraphicsInstanceDesc
	{
		String AppName;
		Math::Vec3c AppVersion;

		String EngineName;
		Math::Vec3c EngineVersion;
	};
}
