/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Definitions.h"

#include <string>
#include <string_view>

namespace MAGE
{
	class String;

	class StringView final
	{
	public:
		StringView() = default;
		StringView(const std::string& str) : m_view(str) {}
		StringView(const std::string_view& str) : m_view(str) {}
		StringView(const String& str);
		StringView(const StringView& str) : m_view(str.m_view) {}
		~StringView() = default;

		operator std::string_view() const { return m_view; }

	private:
		std::string_view m_view;
	};
}