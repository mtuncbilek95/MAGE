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

		void NonReflectedFunction() 
		{
			printf("NonReflectedFunction\n");
			printf("mNonReflectedVariable: %d\n", mNonReflectedVariable);
			printf("mReflectedVariable: %d\n", mReflectedVariable);
			printf("mReflectedString: %s\n", mReflectedString.c_str());
		}

	private:
		int mNonReflectedVariable = 32;

		REFLECT_MEMBER();
		int mReflectedVariable = 42;

		REFLECT_MEMBER();
		String mReflectedString = "Hello, World!";
	};
}

