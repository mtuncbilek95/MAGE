#include "Entity.h"

#include "Engine/WorldSystem/EntityManager.h"

namespace MAGE
{
	Entity::Entity(EnTTHandle handle, EntityManager* manager) : m_handle(handle), m_manager(manager)
	{
		printf("Entity created\n");
	}

	void Entity::Destroy()
	{
		m_manager->DestroyEntity(*this);
		m_handle = entt::null;
	}

	bool Entity::IsValid() const noexcept
	{
		return m_handle != entt::null;
	}
}
