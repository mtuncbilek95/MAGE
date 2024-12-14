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
	class DXSwapchain;

	struct RTVProps final
	{
		Math::Vec2i imgSize{ 1920, 1080 };
		DXGI_FORMAT texFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
		D3D11_USAGE usage = D3D11_USAGE_DEFAULT;
		UINT bindFlag = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		DXGI_FORMAT dsvFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
		D3D11_RTV_DIMENSION dsvFlag = D3D11_RTV_DIMENSION_TEXTURE2D;
		bool hasSRV = true;
		DXGI_FORMAT srvFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
		D3D11_SRV_DIMENSION srvFlag = D3D11_SRV_DIMENSION_TEXTURE2D;
	};

	class DXRenderTarget final : public DXObject
	{
	public:
		DXRenderTarget() = default;
		~DXRenderTarget() override final = default;

		b8 Initialize(const RTVProps& desc, DXDevice* device);
		b8 Initialize(DXSwapchain* swapchain);
		void Destroy() override final;

		ID3D11Texture2D* GetBuffer() const { return m_buffer.Get(); }
		ID3D11RenderTargetView* GetRTV() const { return m_view.Get(); }
		ID3D11ShaderResourceView* GetSRV() const { return m_srv.Get(); }

	private:
		ComPtr<ID3D11Texture2D> m_buffer;
		ComPtr<ID3D11RenderTargetView> m_view;
		ComPtr<ID3D11ShaderResourceView> m_srv;
	};
}