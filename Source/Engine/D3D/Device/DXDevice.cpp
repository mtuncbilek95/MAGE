#include "DXDevice.h"

#include "Engine/D3D/Core/DXAssert.h"
#include "Engine/D3D/Instance/DXInstance.h"

namespace Mage
{
	b8 DXDevice::Initialize(DXInstance* instance)
	{
		m_instance = instance->GetDXInstance();
		m_adapter = instance->GetDXAdapter();

		UINT creationFlags = 0;
#if defined(MAGE_DEBUG)
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };
		D3D_FEATURE_LEVEL selectedFeatureLevel;
		if (!Assertion::DXAssert(D3D11CreateDevice(m_adapter, D3D_DRIVER_TYPE_UNKNOWN, nullptr, creationFlags,
			featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, m_device.ReleaseAndGetAddressOf(), &selectedFeatureLevel,
			m_context.ReleaseAndGetAddressOf()), "DXDevice"))
			return false;

		return true;
	}

	void DXDevice::Destroy()
	{
	}
}