#include "DXInput.h"

#include "Engine/D3D/Core/DXAssert.h"
#include "Engine/D3D/Device/DXDevice.h"
#include "Engine/D3D/Shader/DXShader.h"

namespace Mage
{
	b8 DXInput::Initialize(const InputProps& desc, DXDevice* device)
	{
		m_rootDevice = device;

		vector<D3D11_INPUT_ELEMENT_DESC> layouts(desc.entries.size());
		for (u32 i = 0; i < desc.entries.size(); ++i)
		{
			layouts[i].SemanticName = desc.entries[i].name.c_str();
			layouts[i].SemanticIndex = desc.entries[i].index;
			layouts[i].Format = desc.entries[i].format;
			layouts[i].InputSlot = desc.entries[i].inputSlot;
			layouts[i].AlignedByteOffset = desc.entries[i].offset;
			layouts[i].InputSlotClass = desc.entries[i].perClass;
			layouts[i].InstanceDataStepRate = desc.entries[i].stepRate;
		}

		if (!Assertion::DXAssert(device->GetDXDevice()->CreateInputLayout(layouts.data(), layouts.size(), desc.shader->GetData()->GetBufferPointer(),
			desc.shader->GetData()->GetBufferSize(), m_layout.GetAddressOf()), "DXInputLayout"))
			return false;
		return true;
	}

	void DXInput::Destroy()
	{
	}
}