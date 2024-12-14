#include "DXSampler.h"

#include "Engine/D3D/Core/DXAssert.h"
#include "Engine/D3D/Device/DXDevice.h"

namespace Mage
{
	b8 DXSampler::Initialize(DXDevice* device)
	{
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; 
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;                    
		samplerDesc.MaxAnisotropy = 1;                        
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0.0f;
		samplerDesc.BorderColor[1] = 0.0f;
		samplerDesc.BorderColor[2] = 0.0f;
		samplerDesc.BorderColor[3] = 0.0f;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		if (!Assertion::DXAssert(device->GetDXDevice()->CreateSamplerState(&samplerDesc, m_sampler.GetAddressOf()), "DXSampler"))
			return false;

		return true;
	}

	void DXSampler::Destroy()
	{
	}
}