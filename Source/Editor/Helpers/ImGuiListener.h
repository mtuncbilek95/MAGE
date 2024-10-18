/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Helpers/Singleton.h>

namespace MAGE
{
	using EventCallback = Function<void()>;

	class ImGuiListener final : public Singleton<ImGuiListener>
	{
	public:
		ImGuiListener() = default;
		~ImGuiListener() = default;

		void RegisterCallback(const EventCallback& callback);
		void RemoveCallback(const EventCallback& callback);
		void Dispatch();

	private:
		Vector<EventCallback> m_callbacks;
	};
}
