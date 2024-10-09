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

namespace MAGE
{
	class Entity;
	/// <summary>
	/// Entity control unit for the world system.
	/// Each scene has its own entity manager.
	/// </summary>
	class EntityManager final
	{
	public:
		EntityManager() = default;
		~EntityManager() = default;

		Entity CreateEntity();
		bool DestroyEntity(Entity& entity);
		EnTTReg& GetRegistry();

	private:
		EnTTReg m_registry;
	};
}
