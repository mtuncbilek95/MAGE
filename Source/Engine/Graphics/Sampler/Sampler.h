#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Object/DeviceObject.h"
#include "Engine/Graphics/Sampler/SamplerDesc.h"

namespace MAGE
{
	class Sampler : public DeviceObject
	{
	public:
		Sampler(const SamplerDesc& desc, GraphicsDevice* pDevice) : DeviceObject(pDevice),
			mMagFilter(desc.MagFilter), mMinFilter(desc.MinFilter), mAddressMode(desc.AddressMode),
			mAnisotropyEnabled(desc.bAnisotropyEnabled), mMaxAnisotropy(desc.MaxAnisotropy), mBorderColor(desc.BorderColor), 
			mCompareEnabled(desc.bCompareEnabled), mCompareOp(desc.CompareOp), mMapMode(desc.MapMode), mMipLODBias(desc.MipLODBias), 
			mMinLOD(desc.MinLOD), mMaxLOD(desc.MaxLOD)
		{}
		virtual ~Sampler() override = default;

		SamplerFilter GetMagFilter() const { return mMagFilter; }
		SamplerFilter GetMinFilter() const { return mMinFilter; }
		SamplerAddressMode GetAddressMode() const { return mAddressMode; }
		b8 IsAnisotropyEnabled() const { return mAnisotropyEnabled; }
		u32 GetMaxAnisotropy() const { return mMaxAnisotropy; }
		SamplerBorderColor GetBorderColor() const { return mBorderColor; }
		b8 IsCompareEnabled() const { return mCompareEnabled; }
		CompareOperation GetCompareOp() const { return mCompareOp; }
		SamplerMapMode GetMapMode() const { return mMapMode; }
		f32 GetMipLODBias() const { return mMipLODBias; }
		f32 GetMinLOD() const { return mMinLOD; }
		f32 GetMaxLOD() const { return mMaxLOD; }

	protected:
		void SetMaxAnisotropy(u32 maxAnisotropy) { mMaxAnisotropy = maxAnisotropy; }

	private:
		SamplerFilter mMagFilter;
		SamplerFilter mMinFilter;
		SamplerAddressMode mAddressMode;
		b8 mAnisotropyEnabled;
		u32 mMaxAnisotropy;
		SamplerBorderColor mBorderColor;
		b8 mCompareEnabled;
		CompareOperation mCompareOp;
		SamplerMapMode mMapMode;
		f32 mMipLODBias;
		f32 mMinLOD;
		f32 mMaxLOD;
	};
}