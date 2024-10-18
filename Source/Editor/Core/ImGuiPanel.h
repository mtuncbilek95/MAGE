/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include <Engine/Core/Core.h>
#include "Editor/Helpers/ImGuiListener.h"

namespace MAGE
{
	class ImGuiPanel
	{
	public:
		virtual ~ImGuiPanel() = default;

		void SetVisibility(bool visible)
		{
			if (visible)
			{
				ImGuiListener::Get().RegisterCallback([this]() { Update(); });
				ImGuiListener::Get().RegisterCallback([this]() { Render(); });
			}
			else
			{
				ImGuiListener::Get().RemoveCallback([this]() { Update(); });
				ImGuiListener::Get().RemoveCallback([this]() { Render(); });
			}
		}

		virtual void Update() = 0;
		virtual void Render() = 0;
	};
}
