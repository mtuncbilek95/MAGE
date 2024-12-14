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
	struct BufferProps final
	{
		u32 size;
		D3D11_USAGE usage;
		D3D11_BIND_FLAG bindFlag;
		u32 cpuAccess;
		SharedMemory data;
	};

	class DXBuffer final : public DXObject
	{
	public:
		DXBuffer() = default;
		~DXBuffer() override final = default;

		b8 Initialize(const BufferProps& desc, DXDevice* device);
		void UpdateData(const SharedMemory& data);
		void Destroy() override final;

		ID3D11Buffer* GetBuffer() const { return m_buffer.Get(); }

	private:
		ComPtr<ID3D11Buffer> m_buffer;
		usize m_size;
	};
}