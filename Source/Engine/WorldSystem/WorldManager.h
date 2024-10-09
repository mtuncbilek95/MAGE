#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	class WorldManager final
	{
	public:
		WorldManager() = default;
		~WorldManager() = default;

		void RegisterScene(const String& name, const Shared<Scene>& scene)
		{
			m_Scenes[name] = scene;
		}

		Shared<Scene> GetScene(const String& name)
		{
			return m_Scenes[name];
		}



	private:
		HashMap<String, Shared<Scene>> m_Scenes;
	};
}
