/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Data/Guid.h"
#include "Engine/Objects/Singleton.h"

#include "../Core/IResource.h"

namespace MAGE
{
	class ResourceFactory final : public Singleton<ResourceFactory>
	{
	public:
		ResourceFactory() = default;
		~ResourceFactory() = default;

		template<typename Type, typename = std::enable_if<std::is_base_of_v<IResource, Type>>>
		void RegisterResource(const Shared<Type>& obj)
		{
			m_resources[obj->GetID()] = obj;
			m_auxillary[obj->GetRelativePath()] = obj->GetID();
		}

		template<typename Type, typename Key>
		Type* GetResource(const Key& key)
		{
			if constexpr (std::is_same_v<Key, Guid>)
				return dynamic_cast<Type*>(m_resources.at(key).get());
			else
				return dynamic_cast<Type*>(m_resources.at(m_auxillary.at(key)).get());
		}

	private:
		hashmap<Guid, Owned<IResource>, GuidHash> m_resources;
		hashmap<string, Guid> m_auxillary;
	};
}
