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
	class Guid final
	{
		friend struct GuidHash;
	public:
		static Guid GenerateID();

	public:
		Guid();
		Guid(u32 a, u16 b, u16 c, u64 d);
		Guid(const Guid& other);
		Guid(const string& str);

		u32 GetA() const { return m_a; }
		u16 GetB() const { return m_b; }
		u16 GetC() const { return m_c; }
		u64 GetD() const { return m_d; }

		string ToString();

		b8 operator==(const Guid& other) const;
		b8 operator!=(const Guid& other) const;

	private:
		u32 m_a;
		u16 m_b;
		u16 m_c;
		u64 m_d;
	};

	struct GuidHash
	{
		usize operator()(const Guid& other) const;
	};
}
