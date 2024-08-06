#include "TextureResource.h"

#include <Engine/Graphics/API/GraphicsAPI.h>
#include <Engine/Graphics/Device/GraphicsDevice.h>

namespace MAGE
{
	TextureResource::TextureResource() : mDevice(GraphicsAPI::GetAPI()->GetDevice()), mDeviceMemory(nullptr), mHostMemory(nullptr)
	{
		CreateInternal();
	}

	void TextureResource::ConnectMemory(s64 deviceMemSize, s64 hostMemSize)
	{
		mDeviceMemory = GraphicsAPI::GetAPI()->GetAvailableDeviceMemory(deviceMemSize);
		mHostMemory = GraphicsAPI::GetAPI()->GetAvailableHostMemory(hostMemSize);
	}

	void TextureResource::CreateImage(const TextureImageDesc& desc)
	{
		mImage = mDevice->CreateTextureImage(desc);
	}

	void TextureResource::CreateView(TextureAspectFlags aspectFlags, TextureViewType viewType)
	{
		u32 arrayLayers = mImage->GetArrayLayers();
		u32 mipLevels = mImage->GetMipLevels();

		mMipData.resize(arrayLayers);
		for (u32 i = 0; i < arrayLayers; ++i)
		{
			mMipData[i].resize(mipLevels);
			for (u32 j = 0; j < mipLevels; ++j)
			{
				TextureViewDesc viewDesc = {};
				viewDesc.AspectFlags = aspectFlags;
				viewDesc.ViewType = viewType;
				viewDesc.MipLevel = j;
				viewDesc.MipCount = 1;
				viewDesc.ArrayCount = 1;
				viewDesc.pTexture = mImage.get();
				viewDesc.ArrayLayer = i;
				mMipData[i][j].pView = mDevice->CreateTextureView(viewDesc);
			}
		}
	}

	void TextureResource::FillData(MemoryBuffer buffer, u32 arrayIndex, u32 mipIndex)
	{
	}

	TextureView* TextureResource::GetView(u32 arrayLevel, u32 mipLevel)
	{
		if (TextureView* view = mMipData[arrayLevel][mipLevel].pView.get())
			return view;

		return nullptr;
	}

	void TextureResource::CreateInternal()
	{
		mCmdPool = mDevice->CreateCommandPool({ CmdPoolType::Graphics });
		mCmdBuffer = mDevice->CreateCommandBuffer({ mCmdPool.get() });
		mFence = mDevice->CreateGraphicsFence(false);
	}

	void TextureResource::Clear()
	{
		mMipData.clear();
		mDeviceMemory = nullptr;
		mHostMemory = nullptr;
		mImage.reset();
		mCmdBuffer.reset();
		mCmdPool.reset();
		mFence.reset();
	}
}
