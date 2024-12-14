#include "DXBuffer.h"

#include "Engine/D3D/Core/DXAssert.h"
#include "Engine/D3D/Device/DXDevice.h"

namespace Mage
{
	b8 DXBuffer::Initialize(const BufferProps& desc, DXDevice* device)
	{
		m_rootDevice = device;
		m_size = desc.size;

		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = static_cast<UINT>(desc.size);
		bufferDesc.Usage = desc.usage;
		bufferDesc.BindFlags = desc.bindFlag;
		bufferDesc.CPUAccessFlags = desc.cpuAccess;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA initData = {};
		if (desc.data.Data())
			initData.pSysMem = desc.data.Data();

		if (!Assertion::DXAssert(device->GetDXDevice()->CreateBuffer(&bufferDesc, desc.data.Data() ? &initData : nullptr, m_buffer.ReleaseAndGetAddressOf()), "DXBuffer"))
			return false;

		return true;
	}

	void DXBuffer::UpdateData(const SharedMemory& data)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT hr = m_rootDevice->GetDXContext()->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (SUCCEEDED(hr)) {
			memcpy(mappedResource.pData, data.Data(), data.Size());
			m_rootDevice->GetDXContext()->Unmap(m_buffer.Get(), 0);
		}
	}

	void DXBuffer::Destroy()
	{
	}
}
