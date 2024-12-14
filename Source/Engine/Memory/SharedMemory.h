/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Definitions/Definitions.h"
#include "Engine/Definitions/StdNames.h"

namespace Mage
{
	class OwnedMemory;

	class SharedMemory final
	{
		typedef void* buf;

	public:
		SharedMemory();
		SharedMemory(buf data, usize size);
		SharedMemory(const SharedMemory& other);
		SharedMemory(SharedMemory&& other) noexcept;
		SharedMemory(const OwnedMemory& other);
		SharedMemory(OwnedMemory&& other) = delete;
		~SharedMemory();

		SharedMemory& operator=(const SharedMemory& other);
		SharedMemory& operator=(const OwnedMemory& other);

		buf Data() const { return m_buffer; }
		usize Size() const { return m_size; }

	private:
		buf m_buffer;
		usize m_size;
	};
}