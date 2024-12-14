/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */
#pragma once

#include "Engine/D3D/Object/DXObject.h"
#include "Engine/Definitions/GlmNames.h"
#include "Engine/Memory/SharedMemory.h"

#include <d3d11.h>
#include <dxgi.h>
#include <wrl.h>

using namespace Microsoft::WRL;

namespace Mage
{
	class DXSampler final : public DXObject
	{
	public:
		DXSampler() = default;
		~DXSampler() override final = default;

		b8 Initialize(DXDevice* device);
		void Destroy() override final;

		ID3D11SamplerState* GetSampler() const { return m_sampler.Get(); }

	private:
		ComPtr<ID3D11SamplerState> m_sampler;
	};
}