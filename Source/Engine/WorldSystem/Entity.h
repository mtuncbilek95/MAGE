/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/WorldSystem/EnttCore.h"
#include "Engine/WorldSystem/EntityManager.h"

namespace MAGE
{
	class Entity final
	{
		friend class EntityManager;
	public:
		Entity() = default;
		Entity(EnTTHandle handle, EntityManager* manager);
		~Entity() = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			return m_manager->GetRegistry().emplace<T>(m_handle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			return m_manager->GetRegistry().get<T>(m_handle);
		}

		template<typename T>
		bool HasComponent() const
		{
			return m_manager->GetRegistry().has<T>(m_handle);
		}

		template<typename T>
		void RemoveComponent()
		{
			m_manager->GetRegistry().remove<T>(m_handle);
		}

		void Destroy();
		bool IsValid() const noexcept;

	private:
		EnTTHandle m_handle{ entt::null };
		EntityManager* m_manager{ nullptr };
	};
}
