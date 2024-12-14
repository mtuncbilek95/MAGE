/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */
#pragma once

#include "Engine/Containers/IObject.h"

#include <d3d11.h>
#include <dxgi.h>
#include <wrl.h>

using namespace Microsoft::WRL;

namespace Mage
{
	class DXInstance;

	class DXDevice final : public IObject
	{
	public:
		DXDevice() = default;
		~DXDevice() override final = default;

		b8 Initialize(DXInstance* instance);
		void Destroy() override final;

		IDXGIFactory* GetDXInstance() const { return m_instance; }
		IDXGIAdapter* GetDXAdapter() const { return m_adapter; }
		ID3D11Device* GetDXDevice() const { return m_device.Get(); }
		ID3D11DeviceContext* GetDXContext() const { return m_context.Get(); }

	private:
		IDXGIFactory* m_instance;
		IDXGIAdapter* m_adapter;
		ComPtr<ID3D11Device> m_device;
		ComPtr<ID3D11DeviceContext> m_context;
	};
}