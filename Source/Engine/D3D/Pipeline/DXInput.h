/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */
#pragma once

#include "Engine/D3D/Object/DXObject.h"

#include <d3d11.h>
#include <dxgi.h>
#include <wrl.h>

using namespace Microsoft::WRL;

namespace Mage
{
	class DXShader;

	struct Entry final
	{
		string name;
		u32 index;
		DXGI_FORMAT format;
		u32 inputSlot;
		u32 offset = D3D11_APPEND_ALIGNED_ELEMENT;
		D3D11_INPUT_CLASSIFICATION perClass = D3D11_INPUT_PER_VERTEX_DATA;
		u32 stepRate = 0;
	};

	struct InputProps final
	{
		vector<Entry> entries;
		DXShader* shader;
	};

	class DXInput final : public DXObject
	{
	public:
		DXInput() = default;
		~DXInput() override final = default;

		ID3D11InputLayout* GetLayout() const { return m_layout.Get(); }

		b8 Initialize(const InputProps& desc, DXDevice* device);
		void Destroy() override final;

	private:
		ComPtr<ID3D11InputLayout> m_layout;
	};
}
