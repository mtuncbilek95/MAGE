/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Resource/Core/Resource.h"

#include "Engine/Vulkan/Buffer/VBuffer.h"

namespace MAGE
{
	class Mesh : public Resource
	{
	public:
		Mesh(const json& desc);
		virtual ~Mesh() override;

		void Serialize() override final;
		void Deserialize() override final;
		void Destroy() override final;

	protected:
		Owned<VBuffer> m_geoBuffer;
	};
}
