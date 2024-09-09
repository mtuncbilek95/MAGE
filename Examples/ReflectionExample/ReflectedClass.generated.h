#pragma once

#include <entt/entt.hpp>
using namespace entt::literals;

#undef GENERATE_MANIFEST
#define GENERATE_MANIFEST \
	friend class TypeResolver<ReflectedClass>; \
	friend class entt::meta_factory<ReflectedClass>



namespace MAGE
{
	class ReflectedClass;

	template<>
	class TypeResolver<ReflectedClass>
	{

	};
}
