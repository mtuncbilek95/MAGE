#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Reflection/ObjectMacros.h>
#include "TestReflection.gen.h"

namespace MAGE
{
	M_CLASS();
	class TestReflection
	{
		GENERATE_OBJECT;
	public:
		TestReflection() = default;
		~TestRefleciton() = default;
	};
}
