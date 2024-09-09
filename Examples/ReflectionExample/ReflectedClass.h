#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Serialization/GenerationMacros.h"
#include "ReflectedClass.generated.h"

namespace MAGE
{
	REFLECT_CLASS();
	class ReflectedClass
	{
		GENERATE_MANIFEST;

	public:
		ReflectedClass() = default;
		~ReflectedClass() = default;

		void NonReflectedFunction() {}

	private:
		int mNonReflectedVariable = 32;

		REFLECT_MEMBER();
		int mReflectedVariable = 42;

		REFLECT_MEMBER();
		String mReflectedString = "Hello, World!";
	};
}

