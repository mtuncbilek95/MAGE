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
#include <d3dcompiler.h>
#include <dxgi.h>
#include <wrl.h>

using namespace Microsoft::WRL;

namespace Mage
{
	enum class ShaderTypes
	{
		Vertex,
		Hull,
		Domain,
		Geometry,
		Compute,
		Pixel
	};

	struct ShaderProps final
	{
		ShaderTypes type;
		wstring path;
		string entry;
		string model;
	};

	class DXShader final : public DXObject
	{
	public:
		DXShader() = default;
		~DXShader() override final = default;

		ID3DBlob* GetData() const { return m_data.Get(); }
		ID3D11VertexShader* GetVShader() const { return m_vertex.Get(); }
		ID3D11HullShader* GetTCShader() const { return m_tesCtrl.Get(); }
		ID3D11DomainShader* GetTEShader() const { return m_tesEval.Get(); }
		ID3D11GeometryShader* GetGShader() const { return m_geo.Get(); }
		ID3D11ComputeShader* GetCShader() const { return m_compute.Get(); }
		ID3D11PixelShader* GetPShader() const { return m_pixel.Get(); }

		b8 Initialize(const ShaderProps& desc, DXDevice* device);
		void Destroy();

	private:
		ComPtr<ID3D11VertexShader> m_vertex;
		ComPtr<ID3D11HullShader> m_tesCtrl;
		ComPtr<ID3D11DomainShader> m_tesEval;
		ComPtr<ID3D11GeometryShader> m_geo;
		ComPtr<ID3D11ComputeShader> m_compute;
		ComPtr<ID3D11PixelShader> m_pixel;

		ComPtr<ID3DBlob> m_data;
	};
}