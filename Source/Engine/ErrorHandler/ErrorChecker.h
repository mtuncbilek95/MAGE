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
	namespace ErrorUtils
	{
		/**
		 * @struct Handler
		 * @brief No need to use them in mid/high level codings.
		 */
		struct Handler final
		{
			static string GetErrorString();
			static void PlatformMessage(const string& title, const char* message, ...);
		};

		/**
		 * @brief Assertion for debug, mostly will be used in low level APIs.
		 * @note In release mode it just closes the app with no log.
		 */
		template<typename...Args>
		inline constexpr void LogAssert(b8 condition, const string& title, const string& message, Args&&...args)
		{
			if (!condition)
				Handler::PlatformMessage(title, message.c_str(), std::forward<Args>(args)...);
		}
	} // namespace ErrorChecker
}


