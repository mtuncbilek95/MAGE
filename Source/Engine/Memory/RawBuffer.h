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

	class ENGINE_API RawBuffer final
	{
		typedef void* buf;
	public:
		RawBuffer();
		RawBuffer(buf pData, usize size);
		RawBuffer(const RawBuffer& pOther);
		RawBuffer(const OwnedBuffer& pOther);
		~RawBuffer();

		RawBuffer(RawBuffer&& pOther) = delete;
		RawBuffer& operator=(RawBuffer&& pOther) = delete;

		RawBuffer& operator=(const RawBuffer& pOther);

		buf Data() const { return mBufferData; }
		usize Size() const { return mBufferSize; }

		operator bool() const { return mBufferData != nullptr; }

	private:
		buf mBufferData;
		usize mBufferSize;
	};
}
