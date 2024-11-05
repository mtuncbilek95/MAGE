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

namespace MAGE
{
	class VDevice;

	class VkObject : public IObject
	{
	public:
		VkObject(VDevice* device);
		virtual ~VkObject() override = default;

		VDevice* GetRootDevice() const { return m_rootDevice; }

		virtual void Destroy() override = 0;

	protected:
		VDevice* m_rootDevice;
	};
}
