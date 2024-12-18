/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Definitions.h"

namespace MAGE
{
	namespace ByteConv
	{
		template<Arithmetic T>
		inline constexpr T MBToByte(T mb) { return mb * 1000.0 * 1000.0; }
		template<Arithmetic T>
		inline constexpr T KBToByte(T kb) { return kb * 1000.0; }
		template<Arithmetic T>
		inline constexpr T ByteToKB(T bytes) { return bytes / 1000.0; }
		template<Arithmetic T>
		inline constexpr T ByteToMB(T bytes) { return bytes / 1000.0 / 1000.0; }

		template<Arithmetic T>
		inline constexpr T MiBToByte(T mib) { return mib << 20; }
		template<Arithmetic T>
		inline constexpr T KiBToByte(T mib) { return mib << 10; }
		template<Arithmetic T>
		inline constexpr T ByteToKiB(T bytes) { return bytes >> 10; }
		template<Arithmetic T>
		inline constexpr T ByteToMiB(T bytes) { return bytes >> 20; }
	}
}
