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
	class OwnedBuffer final
	{
		typedef void* buf;
	public:
		OwnedBuffer();
		OwnedBuffer(buf data, usize size);
		OwnedBuffer(u8* begin, u8* end);
		OwnedBuffer(const OwnedBuffer& other);
		OwnedBuffer(OwnedBuffer&& other) noexcept;
		~OwnedBuffer();

		OwnedBuffer& operator=(OwnedBuffer& other);
		OwnedBuffer& operator=(OwnedBuffer&& other) noexcept;

		buf Data() const { return m_bufferData; }
		usize Size() const { return m_bufferSize; }

		operator bool() const { return m_bufferData != nullptr; }

	protected:
		void Allocate(const buf data, const usize size);
	private:
		buf m_bufferData;
		usize m_bufferSize;
	};
} // namespace IOAlloc
