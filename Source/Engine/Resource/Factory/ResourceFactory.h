/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Objects/Singleton.h"
#include "Engine/Data/Guid.h"
#include "Engine/Resource/Core/Resource.h"

namespace MAGE
{
	class ResourceFactory final : public Singleton<ResourceFactory>
	{
		friend class ResourceHandler;

	public:
		ResourceFactory() = default;
		~ResourceFactory() = default;

		template<typename T = Resource, typename Key = Guid, typename = std::enable_if<std::is_base_of_v<Resource, T> && (std::is_same_v<Guid, Key> || std::is_same_v<string, Key>)>>
		auto GetResourceObject(const Key& key) -> typename std::conditional<std::is_void<T>::value, Resource*, T*>::type
		{
			if constexpr (std::is_same_v<string, Key>)
			{
				auto it = m_auxillary.find(key);
				if (it != m_auxillary.end())
					return dynamic_cast<T*>(&*m_resources[it->second]);
			}
			else
				return dynamic_cast<T*>(&*m_resources[key]);

			return nullptr;
		}

	protected:
		void RegisterResource(const Shared<Resource>& obj)
		{
			m_resources[obj->GetGuid()] = obj;
			m_auxillary.emplace(obj->GetPath() + obj->GetResourceName() + "." + obj->GetType(), obj->GetGuid());
		}

	private:
		hashmap<Guid, Shared<Resource>, GuidHash> m_resources;
		map<string, Guid> m_auxillary;
	};
}
