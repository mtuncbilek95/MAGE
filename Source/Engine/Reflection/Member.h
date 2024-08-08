#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Reflection/MemberMode.h>
#include <Engine/Reflection/Type.h>

namespace MAGE
{
	/**
	 * @class Member
	 * 
	 * @brief Member class is the root primitive member of any class or struct. 
	 */
	class ENGINE_API Member final
	{
		friend class TypeDispatcher;
	public:
		Member(const String& name, MemberMode mode, Type* type, u32 offset) : mName(name), mMemberMode(mode), mMemberType(type), mOffset(offset) {}
		~Member() = default;

		FORCEINLINE const String& GetName() const { return mName; }
		FORCEINLINE MemberMode GetMemberMode() const { return mMemberMode; }
		FORCEINLINE Type* GetMemberType() const { return mMemberType; }
		FORCEINLINE u32 GetOffset() const { return mOffset; }
		
		template<typename T>
		void SetValue(void* object, const T& value) const noexcept
		{
			*(T*)((u8*)object + mOffset) = value;
		}

		template<typename T>
		T GetValue(void* object) const noexcept
		{
			return *(T*)((u8*)object + mOffset);
		}

		template<typename T>
		T* GetPtr(void* object) const noexcept
		{
			return (T*)((u8*)object + mOffset);
		}

	private:
		String mName;
		MemberMode mMemberMode;
		Type* mMemberType;
		u32 mOffset;
	};
}

