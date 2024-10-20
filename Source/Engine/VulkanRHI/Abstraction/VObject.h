/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include <vulkan/vulkan.h>

namespace MAGE
{
	class VDevice;

	class VObject
	{
	public:
		VObject(VDevice* device) : m_rootDevice(device) {}
		virtual ~VObject() = default;

		virtual void Destroy() = 0;

	protected:
		VDevice* m_rootDevice;
	};
}
