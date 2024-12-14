#include "DXImage.h"

#include "Engine/D3D/Core/DXAssert.h"
#include "Engine/D3D/Device/DXDevice.h"

namespace Mage
{
	b8 DXImage::Initialize(const ImageProps& desc, DXDevice* device)
	{
		m_rootDevice = device;

		D3D11_TEXTURE2D_DESC texInfo = {};
		texInfo.Width = desc.imgSize.x;
		texInfo.Height = desc.imgSize.y;
		texInfo.MipLevels = 1;
		texInfo.ArraySize = 1;
		texInfo.Format = desc.texFormat;
		texInfo.SampleDesc.Count = 1;
		texInfo.SampleDesc.Quality = 0;
		texInfo.Usage = desc.usage;
		texInfo.BindFlags = desc.bindFlag;

		D3D11_SUBRESOURCE_DATA data = {};
		if (desc.data.Data())
		{
			data.pSysMem = desc.data.Data();
			data.SysMemPitch = desc.pitch;
		}

		if (!Assertion::DXAssert(device->GetDXDevice()->CreateTexture2D(&texInfo, desc.data.Data() ? &data : nullptr, m_buffer.ReleaseAndGetAddressOf()), "DXTexture"))
			return false;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = desc.texFormat;
		srvDesc.ViewDimension = desc.srvFlag;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = texInfo.MipLevels;

		if (!Assertion::DXAssert(device->GetDXDevice()->CreateShaderResourceView(m_buffer.Get(), &srvDesc, m_srv.ReleaseAndGetAddressOf()), "DXTexture"))
			return false;

		return true;
	}

	void DXImage::Destroy()
	{
	}
}