#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Sampler/SamplerFilter.h"
#include "Engine/Graphics/Sampler/SamplerAddressMode.h"
#include "Engine/Graphics/Sampler/SamplerBorderColor.h"
#include "Engine/Graphics/Sampler/SamplerMapMode.h"
#include "Engine/Graphics/Core/CompareOperation.h"

namespace MAGE
{
	struct SamplerDesc
	{
		SamplerFilter MagFilter;
		SamplerFilter MinFilter;
		SamplerAddressMode AddressMode;
		b8 bAnisotropyEnabled;
		u32 MaxAnisotropy;
		SamplerBorderColor BorderColor;
		b8 bCompareEnabled;
		CompareOperation CompareOp;
		SamplerMapMode MapMode;
		f32 MipLODBias;
		f32 MinLOD;
		f32 MaxLOD;
	};
}