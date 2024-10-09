#include "EntityManager.h"

#include "Engine/WorldSystem/Entity.h"

namespace MAGE
{
	Entity EntityManager::CreateEntity()
	{
		EnTTHandle handle = m_registry.create();
		return Entity(handle, this);
	}

	bool EntityManager::DestroyEntity(Entity& entity)
	{
		if (entity.IsValid())
		{
			m_registry.destroy(entity.m_handle);
			entity.m_handle = entt::null;
			return true;
		}
		return false;
	}

	EnTTReg& EntityManager::GetRegistry()
	{
		return m_registry;
	}
}
