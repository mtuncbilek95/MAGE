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
	struct ImageProps final
	{
		Math::Vec2i imgSize{ 1920, 1080 };
		DXGI_FORMAT texFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		D3D11_USAGE usage = D3D11_USAGE_DEFAULT;
		UINT bindFlag = D3D11_BIND_SHADER_RESOURCE;
		D3D11_SRV_DIMENSION srvFlag = D3D11_SRV_DIMENSION_TEXTURE2D;
		SharedMemory data;
		UINT pitch = 0;
	};

	class DXImage final : public DXObject
	{
	public:
		DXImage() = default;
		~DXImage() override final = default;

		b8 Initialize(const ImageProps& desc, DXDevice* device);
		void Destroy() override final;

		ID3D11Texture2D* GetBuffer() const { return m_buffer.Get(); }
		ID3D11ShaderResourceView* GetSRV() const { return m_srv.Get(); }

	private:
		ComPtr<ID3D11Texture2D> m_buffer;
		ComPtr<ID3D11ShaderResourceView> m_srv;
	};
}