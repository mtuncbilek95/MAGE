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
	class OwnedMemory final
	{
		typedef void* buf;
	
	public:
		OwnedMemory();
		OwnedMemory(buf data, usize size);
		OwnedMemory(u8* begin, u8* end);
		OwnedMemory(const OwnedMemory& other);
		OwnedMemory(OwnedMemory&& other) noexcept;
		~OwnedMemory();

		OwnedMemory& operator=(OwnedMemory& other);
		OwnedMemory& operator=(OwnedMemory&& other) noexcept;

		buf Data() const { return m_buffer; }
		usize Size() const { return m_size; }

		operator bool() const { return m_buffer != nullptr; }

	protected:
		void Allocate(const buf data, const usize size);
		
	private:
		buf m_buffer;
		usize m_size;
	};
}