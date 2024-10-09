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
	template<Arithmetic T>
	inline const T MbToByte(T mb) { return mb * 1000.0 * 1000.0; }
	template<Arithmetic T>
	inline const T KbToByte(T kb) { return kb * 1000.0; }
	template<Arithmetic T>
	inline const T ByteToKb(T bytes) { return bytes / 1000.0; }
	template<Arithmetic T>
	inline const T ByteToMb(T bytes) { return bytes / 1000.0 / 1000.0; }
} // namespace Helpers
