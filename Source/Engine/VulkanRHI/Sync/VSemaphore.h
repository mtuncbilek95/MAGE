/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "../Abstraction/VObject.h"

namespace MAGE
{
	class VSemaphore final : public VObject
	{
	public:
		VSemaphore(VDevice* device);
		~VSemaphore() override final;

		void Destroy() override final;

		 inline VkSemaphore GetSemaphore() const { return m_semaphore; }

	private:
		VkSemaphore m_semaphore;
	};
}
