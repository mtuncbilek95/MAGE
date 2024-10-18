/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include <Engine/Core/Core.h>

#include "Editor/Core/ImGuiPanel.h"

namespace MAGE
{
	class EntityWindow final : public ImGuiPanel
	{
	public:
		EntityWindow();
		~EntityWindow() override;

		void Update() override;
		void Render() override;

	private:
		bool m_dirty = false;
	};
}