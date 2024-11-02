/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include "Texture.h"

namespace MAGE
{
	class Texture2D final : public Texture // TODO: Add Serialization interface
	{
	public:
		Texture2D();
		~Texture2D() override final;

		void CreateImage(const String& imagePath) override final;
		void CreateView() override final;
	};
}
