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
		OwnedBuffer(buf pData, usize size);
		OwnedBuffer(u8* pBegin, u8* pEnd);
		OwnedBuffer(const OwnedBuffer& pOther);
		OwnedBuffer(OwnedBuffer&& pOther) noexcept;
		~OwnedBuffer();

		OwnedBuffer& operator=(OwnedBuffer& pOther);
		OwnedBuffer& operator=(OwnedBuffer&& pOther) noexcept;

		buf Data() const { return mBufferData; }
		usize Size() const { return mBufferSize; }

		operator bool() const { return mBufferData != nullptr; }

	protected:
		void Allocate(const buf pData, const usize size);
	private:
		buf mBufferData;
		usize mBufferSize;
	};
} // namespace IOAlloc
