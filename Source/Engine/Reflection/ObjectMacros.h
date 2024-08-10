#pragma once

#include <Engine/Core/StdFix.h>
#include <Engine/Reflection/Class.h>
#include <Engine/Reflection/EnumType.h>
#include <Engine/Reflection/EnumValue.h>
#include <Engine/Reflection/Member.h>
#include <Engine/Reflection/MemberMode.h>
#include <Engine/Reflection/Type.h>
#include <Engine/Reflection/TypeMode.h>

namespace MAGE
{
	class Type;
	template<typename T>
	class TypeAccessor {};

#define typeof(type) TypeAccessor<##type>::GetType()

#define M_CLASS(...)
#define M_STRUCT(...)
#define M_ENUM(...)
#define M_FUNCTION(...)
#define M_PROPERTY(...)

#define GENERATE_OBJECT

#define GENERATE_MEMBER(type) \
	template<> \
	class TypeAccessor<type> \
	{ \
		friend class TypeDispatcher; \
	public: \
		static Type* GetType() { return mType; } \
	private: \
		static void SetType(Type* type) { mType = type; } \
		static Type** GetTypeAddress() { return &mType; } \
	private: \
		inline static Type* mType = nullptr; \
	};

	GENERATE_MEMBER(i8);
	GENERATE_MEMBER(i16);
	GENERATE_MEMBER(i32);
	GENERATE_MEMBER(i64);
	GENERATE_MEMBER(u8);
	GENERATE_MEMBER(u16);
	GENERATE_MEMBER(u32);
	GENERATE_MEMBER(u64);
	GENERATE_MEMBER(f32);
	GENERATE_MEMBER(f64);
}


