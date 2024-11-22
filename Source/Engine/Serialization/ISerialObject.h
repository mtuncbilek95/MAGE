/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	class ISerialObject
	{
	public:
		virtual ~ISerialObject() = default;

		virtual void Serialize() = 0;
		virtual void Deserialize() = 0;
	};
}
