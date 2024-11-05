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
	class OwnedBuffer;

	class RawBuffer final
	{
		typedef void* buf;
	public:
		RawBuffer();
		RawBuffer(buf data, usize size);
		RawBuffer(const RawBuffer& other);
		RawBuffer(const OwnedBuffer& other);
		~RawBuffer();

		RawBuffer(RawBuffer&& other) = delete;
		RawBuffer& operator=(RawBuffer&& other) = delete;

		RawBuffer& operator=(const RawBuffer& other);

		buf Data() const { return m_bufferData; }
		usize Size() const { return m_bufferSize; }

		operator bool() const { return m_bufferData != nullptr; }

	private:
		buf m_bufferData;
		usize m_bufferSize;
	};
}
