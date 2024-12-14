#include "DXShader.h"

#include "Engine/D3D/Core/DXAssert.h"
#include "Engine/D3D/Device/DXDevice.h"

#include <spdlog/spdlog.h>

namespace Mage
{
	b8 DXShader::Initialize(const ShaderProps& desc, DXDevice* device)
	{
        m_rootDevice = device;
        ComPtr<ID3DBlob> errorBlob;

        if (!Assertion::DXAssert(D3DCompileFromFile(desc.path.c_str(), nullptr, nullptr, desc.entry.c_str(), desc.model.c_str(),
            D3DCOMPILE_ENABLE_STRICTNESS, 0, &m_data, &errorBlob), "DXCompileShader"))
        {
            spdlog::warn("{}", (char*)errorBlob->GetBufferPointer());
            return false;
        }

        HRESULT hr = {};

        switch (desc.type)
        {
        case ShaderTypes::Vertex:
            hr = m_rootDevice->GetDXDevice()->CreateVertexShader(m_data->GetBufferPointer(), m_data->GetBufferSize(), nullptr, &m_vertex);
            break;
        case ShaderTypes::Pixel:
            hr = m_rootDevice->GetDXDevice()->CreatePixelShader(m_data->GetBufferPointer(), m_data->GetBufferSize(), nullptr, &m_pixel);
            break;
        case ShaderTypes::Geometry:
            hr = m_rootDevice->GetDXDevice()->CreateGeometryShader(m_data->GetBufferPointer(), m_data->GetBufferSize(), nullptr, &m_geo);
            break;
        case ShaderTypes::Hull:
            hr = m_rootDevice->GetDXDevice()->CreateHullShader(m_data->GetBufferPointer(), m_data->GetBufferSize(), nullptr, &m_tesCtrl);
            break;
        case ShaderTypes::Domain:
            hr = m_rootDevice->GetDXDevice()->CreateDomainShader(m_data->GetBufferPointer(), m_data->GetBufferSize(), nullptr, &m_tesEval);
            break;
        case ShaderTypes::Compute:
            hr = m_rootDevice->GetDXDevice()->CreateComputeShader(m_data->GetBufferPointer(), m_data->GetBufferSize(), nullptr, &m_compute);
            break;
        }

        if (!Assertion::DXAssert(hr, "DXCreateShader"))
            return false;

        return true;
	}

	void DXShader::Destroy()
	{
	}
}