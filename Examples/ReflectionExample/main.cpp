#include "Engine/Core/Core.h"

using namespace MAGE;

#include <entt/entt.hpp>
using namespace entt::literals;

#include "ReflectedClass.h"

int main(i32 argC, char** argV)
{
	ReflectedClass::TypeRegistry::Register();

	//entt::meta_type by_id = entt::resolve("ReflectedClass"_hs);

	//std::shared_ptr<ReflectedClass> reflectedClass;

	//entt::meta_any instance = by_id.construct();
	//if (instance) {
	//	// If construction succeeded, create a shared_ptr
	//	reflectedClass = std::shared_ptr<ReflectedClass>(static_cast<ReflectedClass*>(instance.data()), [by_id](ReflectedClass* ptr) {});
	//}

	//reflectedClass->NonReflectedFunction();
}