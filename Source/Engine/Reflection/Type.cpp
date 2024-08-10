#include "Type.h"

#include <Engine/Reflection/Member.h>

namespace MAGE
{
	Member* Type::GetMember(const String& name) const
	{
		for (Member* member : mMembers)
			if (member->GetName() == name)
				return member;

		return nullptr;
	}

	void Type::RegisterMember(Member* member)
	{
		mMembers.push_back(member);
	}

	void Type::RegisterEnumValue(const String& name, i64 value)
	{
		mEnumValues.push_back({ name, value });
	}

	void Type::SetSuperType(Type* type)
	{
		mSuperType = type;
	}
}
