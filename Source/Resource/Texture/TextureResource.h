#pragma once

#include <Engine/Core/Core.h>
#include <Resource/Core/ResourceSubObject.h>

#include <Engine/Graphics/Sync/Fence.h>
#include <Engine/Graphics/Texture/TextureImage.h>
#include <Engine/Graphics/Command/CmdBuffer.h>
#include <Engine/Graphics/Command/CmdPool.h>
#include <Engine/Graphics/Texture/TextureView.h>

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
		~TextureResource() override = default;

		void ConnectMemory(s64 deviceMemSize, s64 hostMemSize);
		void CreateImage(const TextureImageDesc& desc);
		void CreateView(TextureAspectFlags AspectFlags, TextureViewType ViewType);
		void FillData(MemoryBuffer buffer, u32 arrayIndex, u32 mipIndex);

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

		DArray<DArray<MipData>> mMipData;
	};
}
