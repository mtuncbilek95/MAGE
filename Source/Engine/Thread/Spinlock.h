/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"

#include <atomic>

namespace MAGE
{
	class Spinlock final
	{
	public:
		void Lock();
		void Unlock();

	private:
		std::atomic_flag m_flag = ATOMIC_FLAG_INIT;
	};
}
