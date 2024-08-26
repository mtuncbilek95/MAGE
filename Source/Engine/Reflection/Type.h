#pragma once

#include <stdexcept>
#include <Engine/Core/Core.h>
#include <Engine/Reflection/TypeMode.h>
#include <Engine/Reflection/EnumValue.h>

namespace MAGE
{
	class Member;

	/**
	 * @class Type
	 * 
	 * @brief Type class is the representation of everything that can be a primitive. For example, 
	 * if the reflectable object is a struct, reflection will know that the type of the struct is a STRUCT. This goes 
	 * for all types such as enums, integers, classes, etc.
	 */
	class Type
	{
		friend class TypeDispatcher;
	public:
		Type() = default;
		~Type() = default;

		FORCEINLINE const String& GetName() const { return mName; }
		FORCEINLINE TypeMode GetType() const { return mType; }
		FORCEINLINE s64 GetSize() const { return mSize; }
		FORCEINLINE Member* GetMember(const String& name) const;

		FORCEINLINE const EnumValue& GetEnumValue(const String& name) const
		{
			for(auto& val : mEnumValues)
				if (val.EnumName == name)
					return val;

			throw std::runtime_error("Enum value not found");
		}

		FORCEINLINE const String& GetEnumName(const i64 value)
		{
			for (auto&[EnumName, EnumVal] : mEnumValues)
				if (EnumVal == value)
					return EnumName;

			throw std::runtime_error("Enum value not found");
		}

		b8 IsSubClassOf(const Type* type) const {
			if (type == nullptr)
				return false;

			if (mSuperType->GetType() == type->GetType())
				return true;

			return mSuperType->IsSubClassOf(type);
		}

	private:
		void RegisterMember(Member* member);
		void RegisterEnumValue(const String& name, i64 value);
		void SetSuperType(Type* type);

	private:
		String mName;
		TypeMode mType;
		u32 mSize;
		Vector<Member*> mMembers; // Only used for structs/classes
		Vector<EnumValue> mEnumValues; // Only used for enums
		Type* mSuperType; // Used for checking the inheritance hierarchy
	};
}