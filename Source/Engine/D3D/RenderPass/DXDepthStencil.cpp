#include "DXDepthStencil.h"

#include "Engine/D3D/Device/DXDevice.h"
#include "Engine/D3D/Core/DXAssert.h"

namespace Mage
{
	b8 DXDepthStencil::Initialize(const DepthProps& desc, DXDevice* device)
	{
		m_rootDevice = device;

		D3D11_TEXTURE2D_DESC depthDesc = {};
		depthDesc.Width = desc.imgSize.x;
		depthDesc.Height = desc.imgSize.y;
		depthDesc.MipLevels = 1;
		depthDesc.ArraySize = 1;
		depthDesc.Format = desc.texFormat;
		depthDesc.SampleDesc.Count = 1;
		depthDesc.SampleDesc.Quality = 0;
		depthDesc.Usage = desc.usage;
		depthDesc.BindFlags = desc.bindFlag;

		if (!Assertion::DXAssert(device->GetDXDevice()->CreateTexture2D(&depthDesc, nullptr, m_buffer.ReleaseAndGetAddressOf()), "DXDSV"))
			return false;

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = desc.dsvFormat;
		dsvDesc.ViewDimension = desc.dsvFlag;
		dsvDesc.Texture2D.MipSlice = 0;

		if (!Assertion::DXAssert(device->GetDXDevice()->CreateDepthStencilView(m_buffer.Get(), &dsvDesc, m_view.ReleaseAndGetAddressOf()), "DXDSV"))
			return false;

		if (desc.hasSRV)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Format = desc.srvFormat;
			srvDesc.ViewDimension = desc.srvFlag;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = 1;

			if (!Assertion::DXAssert(device->GetDXDevice()->CreateShaderResourceView(m_buffer.Get(), &srvDesc, m_srv.ReleaseAndGetAddressOf()), "DXDSV"))
				return false;
		}

		D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
		depthStencilDesc.DepthEnable = TRUE;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthStencilDesc.StencilEnable = FALSE;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		if (!Assertion::DXAssert(device->GetDXDevice()->CreateDepthStencilState(&depthStencilDesc, m_state.ReleaseAndGetAddressOf()), "DXDSV"))
			return false;

		return true;
	}

	void DXDepthStencil::Destroy()
	{
	}
}