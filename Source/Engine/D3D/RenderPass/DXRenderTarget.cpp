#include "DXRenderTarget.h"

#include "Engine/D3D/Device/DXDevice.h"
#include "Engine/D3D/Swapchain/DXSwapchain.h"
#include "Engine/D3D/Core/DXAssert.h"

namespace Mage
{
	b8 DXRenderTarget::Initialize(const RTVProps& desc, DXDevice* device)
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

		if (!Assertion::DXAssert(device->GetDXDevice()->CreateTexture2D(&depthDesc, nullptr, m_buffer.ReleaseAndGetAddressOf()), "DXRTV"))
			return false;

		D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		rtvDesc.Format = desc.dsvFormat;
		rtvDesc.ViewDimension = desc.dsvFlag;
		rtvDesc.Texture2D.MipSlice = 0;

		if (!Assertion::DXAssert(device->GetDXDevice()->CreateRenderTargetView(m_buffer.Get(), &rtvDesc, m_view.ReleaseAndGetAddressOf()), "DXRTV"))
			return false;

		if (desc.hasSRV) {
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Format = desc.srvFormat;
			srvDesc.ViewDimension = desc.srvFlag;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = 1;

			if (!Assertion::DXAssert(device->GetDXDevice()->CreateShaderResourceView(m_buffer.Get(), &srvDesc, m_srv.ReleaseAndGetAddressOf()), "DXRTV"))
				return false;
		}

		return true;
	}

	b8 DXRenderTarget::Initialize(DXSwapchain* swapchain)
	{
		m_rootDevice = swapchain->GetRootDevice();

		if (!Assertion::DXAssert(swapchain->GetSwapchain()->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(m_buffer.ReleaseAndGetAddressOf())), "DXRTV"))
			return false;

		if (!Assertion::DXAssert(swapchain->GetDevice()->CreateRenderTargetView(m_buffer.Get(), nullptr, m_view.ReleaseAndGetAddressOf()), "DXRTV"))
			return false;

		return true;
	}

	void DXRenderTarget::Destroy()
	{
	}
}