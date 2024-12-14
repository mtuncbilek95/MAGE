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
	class IndWindow;

	struct SwapchainProps final
	{
		Math::Vec2i imgRes = { 1920, 1080 };
		IndWindow* window;
	};

	class DXSwapchain final : public DXObject
	{
	public:
		DXSwapchain() = default;
		~DXSwapchain() override final = default;

		b8 Initialize(const SwapchainProps& desc, DXDevice* device);
		void Present(b8 synced);
		void Destroy() override final;

		IDXGISwapChain* GetSwapchain() const { return m_swapchain.Get(); }
		ID3D11Device* GetDevice() const { return m_device; }

	private:
		ComPtr<IDXGISwapChain> m_swapchain;
		ID3D11Device* m_device;
	};
}