#pragma once

#include <entt/entt.hpp>
using namespace entt::literals;

#undef GENERATE_MANIFEST
#define GENERATE_MANIFEST \
	friend class entt::meta_factory<ReflectedClass>; \
	public: \
		class TypeRegistry \
		{ \
		public: \
			static void Register() \
			{ \
				entt::meta<ReflectedClass>() \
					.type("ReflectedClass"_hs) \
					.data<&ReflectedClass::mReflectedVariable>("ReflectedVariable"_hs) \
					.data<&ReflectedClass::mReflectedString>("ReflectedString"_hs); \
			} \
		}; \
	private: \
		friend class TypeRegistry
