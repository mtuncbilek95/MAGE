#pragma once

#include "Engine/Core/Definitions.h"
#include "Engine/Core/StdFix.h"

namespace MAGE
{
	/**
	* @class ENGINE_API Guid
	* @brief A class ENGINE_API that represents a unique identifier.
	*/
	class ENGINE_API Guid final
	{
	public:
		Guid();
		Guid(u32 a, u16 b, u16 c, u64 d);
		Guid(const Guid& other);
		Guid(const String& str);

		u32& GetA() { return mA; }
		u16& GetB() { return mB; }
		u16& GetC() { return mC; }
		u64& GetD() { return mD; }

		String ToString();

		b8 operator==(const Guid& other) const;
		b8 operator!=(const Guid& other) const;

	private:
		u32 mA;
		u16 mB;
		u16 mC;
		u64 mD;
	};
}
