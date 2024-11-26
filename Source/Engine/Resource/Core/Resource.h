/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Objects/IObject.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace MAGE
{
	class Resource : public IObject
	{
	public:
		Resource(const json& desc);
		virtual ~Resource() = default;

		string GetResourceName() const { return m_props["Properties"]["Name"].get<string>(); }
		string GetGuid() const { return m_props["Properties"]["ID"].get<string>(); }
		string GetLastModified() const { return m_props["Properties"]["LastModified"].get<string>(); }
		string GetPath() const { return m_props["Properties"]["Path"].get<string>(); }
		string GetType() const { return m_props["Properties"]["Type"].get<string>(); }

		virtual void Serialize() = 0;
		virtual void Deserialize() = 0;

		virtual void Destroy() override = 0;

	protected:
		json m_props;
	};
}
