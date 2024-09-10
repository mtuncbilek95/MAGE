#include "Engine/Core/Core.h"

using namespace MAGE;

#include <entt/entt.hpp>
using namespace entt::literals;

#include "ReflectedClass.h"

int main(i32 argC, char** argV)
{
	ReflectedClass::TypeRegistry::Register();

	auto by_id = entt::resolve("ReflectedClass"_hs);
	auto reflected_variable = by_id.data("ReflectedVariable"_hs);
	auto test = reflected_variable.get(by_id.construct()).cast<int>();

	printf("ReflectedVariable: %d\n", test);
}