/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Helpers/Singleton.h"

#include "Engine/Window/IndWindow.h"

namespace MAGE
{
	namespace Manager
	{
		class Window final : public Singleton<Window>
		{
		public:
			Window() = default;
			~Window() override = default;

			void InitWindow(const IndWindowDesc& props) { m_window = MakeOwned<IndWindow>(props); }
			void DestroyWindow() { m_window->Destroy(); }
			IndWindow& GetWindow() { return *m_window; }

		private:
			Owned<IndWindow> m_window;
		};
	}
}
