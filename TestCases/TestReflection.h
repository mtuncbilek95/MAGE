#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Reflection/ObjectMacros.h>
#include "TestReflection.gen.h"

namespace MAGE
{
	M_STRUCT();
	struct TestStruct
	{
		GENERATE_OBJECT;

		int x;
		int y;
	};

	M_CLASS();
	class TestClass
	{
		GENERATE_OBJECT;
	public:
		TestClass();
		~TestClass();
	};
}
