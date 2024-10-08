/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/WorldSystem/Entity.h"
#include "Engine/WorldSystem/EntityManager.h"

namespace MAGE
{
	class Scene final
	{
	public:
		Scene() = default;
		~Scene() = default;

		EntityManager& GetEntityManager();

	private:
		HashMap<String, Entity> m_entities;
	};
}
