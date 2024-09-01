#pragma once

#include "Engine/Core/Core.h"
#include "Resource/Core/ResourceSubObject.h"
#include "Resource/Texture/TextureResourceDesc.h"

#include "Engine/Graphics/Sync/Fence.h"
#include "Engine/Graphics/Texture/TextureImage.h"
#include "Engine/Graphics/Command/CmdBuffer.h"
#include "Engine/Graphics/Command/CmdPool.h"
#include "Engine/Graphics/Texture/TextureView.h"

namespace MAGE
{
	class RESOURCE_API TextureResource : public ResourceSubObject
	{
		struct MipData
		{
			SharedPtr<TextureView> pView;
			SharedPtr<GraphicsBuffer> pStageBuffer;
		};

	public:
		TextureResource();
		~TextureResource() override;

		void ConnectMemory(s64 deviceMemSize, s64 hostMemSize);
		void CreateImage(const TextureResourceDesc& desc);
		void CreateView(TextureAspectFlags AspectFlags, TextureViewType ViewType);
		void FillData(MemoryBuffer buffer, u32 arrayIndex, u32 mipIndex, u64 offset);
		void UpdateBarrier(TextureImageBarrier barrier);

		TextureView* GetView(u32 arrayLevel, u32 mipLevel);

	private:
		void CreateInternal();
		void Clear();

	private:
		SharedPtr<Fence> mFence;
		SharedPtr<TextureImage> mImage;
		SharedPtr<CmdBuffer> mCmdBuffer;
		SharedPtr<CmdPool> mCmdPool;

		GraphicsMemory* mDeviceMemory;
		GraphicsMemory* mHostMemory;

		GraphicsDevice* mDevice;

		Vector<Vector<MipData>> mMipData;
	};
}
