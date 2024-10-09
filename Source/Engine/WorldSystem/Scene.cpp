#include "Scene.h"

#include "Engine/WorldSystem/EntityManager.h"

namespace MAGE
{
	static EntityManager s_entityManager;

	EntityManager& Scene::GetEntityManager()
	{
		return s_entityManager;
	}
}
