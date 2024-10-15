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
	template <typename T>
	class Singleton
	{
	public:
		Singleton() = default;
		virtual ~Singleton() = default;

		static T& Get()
		{
			if (!m_Instance)
				m_Instance = MakeShared<T>();

			return *m_Instance;
		}

	private:
		inline static Shared<T> m_Instance = nullptr;
	};
}
