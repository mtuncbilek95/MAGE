#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Reflection/PrimitiveType.h>

namespace MAGE
{
	class Primitive
	{
	public:
		Primitive() = default;
		~Primitive() = default;

		FORCEINLINE const String& GetName() const { return mName; }
		FORCEINLINE PrimitiveType GetType() const { return mType; }
		FORCEINLINE s64 GetOffset() const { return mOffset; }

	private:
		String mName;
		PrimitiveType mType;
		s64 mOffset;


	};
}
