/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */
#pragma once

#include "Engine/Definitions/Definitions.h"
#include "Engine/Definitions/StdNames.h"
#include "Engine/Containers/Guid.h"

namespace Mage
{
	class IObject
	{
	public:
		IObject() : m_id(GuidHelper::GenerateID()) {}
		virtual ~IObject() = default;

		const Guid& GetID() { return m_id; }
		virtual void Destroy() = 0;

	private:
		Guid m_id;
	};
}