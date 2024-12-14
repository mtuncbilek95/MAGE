#include "DXInstance.h"

#include "Engine/D3D/Core/DXAssert.h"

namespace Mage
{
	b8 DXInstance::Initialize()
	{
		if (!Assertion::DXAssert(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)m_instance.GetAddressOf()), "DXInstance"))
			return false;

		if (!Assertion::DXAssert(m_instance->EnumAdapters(0, m_adapter.ReleaseAndGetAddressOf()), "DXInstance"))
			return false;

		return true;
	}

	void DXInstance::Destroy()
	{
	}
}
