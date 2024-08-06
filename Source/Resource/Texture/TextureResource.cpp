#include "TextureResource.h"

#include <Engine/Graphics/API/GraphicsAPI.h>
#include <Engine/Graphics/Device/GraphicsDevice.h>

namespace MAGE
{
	TextureResource::TextureResource() : mDevice(GraphicsAPI::GetAPI()->GetDevice()), mDeviceMemory(nullptr), mHostMemory(nullptr)
	{
		CreateInternal();
	}

	TextureResource::~TextureResource()
	{
		Clear();
	}

	void TextureResource::ConnectMemory(s64 deviceMemSize, s64 hostMemSize)
	{
		mDeviceMemory = GraphicsAPI::GetAPI()->GetAvailableDeviceMemory(deviceMemSize);
		mHostMemory = GraphicsAPI::GetAPI()->GetAvailableHostMemory(hostMemSize);
	}

	void TextureResource::CreateImage(const TextureResourceDesc& desc)
	{
		TextureImageDesc imgDesc = {};
		imgDesc.CreateFlags = desc.CreateFlags;
		imgDesc.ImageFormat = desc.ImageFormat;
		imgDesc.ImageSize = desc.ImageSize;
		imgDesc.ImageType = desc.ImageType;
		imgDesc.MipLevels = 1; // TODO: Need improvement
		imgDesc.ArrayLayers = 1; // TODO: Need improvement
		imgDesc.Samples = desc.Samples;
		imgDesc.UsageFlags = TextureUsageFlags::SampledImage | TextureUsageFlags::TransferDestination;
		imgDesc.pRequestMemory = mDeviceMemory;

		mImage = mDevice->CreateTextureImage(imgDesc);
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

	void TextureResource::FillData(MemoryBuffer buffer, u32 arrayIndex, u32 mipIndex, u64 offset)
	{
		GraphicsBufferDesc bufferDesc = {};
		bufferDesc.BlockLength = 1;
		bufferDesc.BlockSize = buffer.GetSize();
		bufferDesc.pRequestMemory = mHostMemory;
		bufferDesc.Usage = GraphicsBufferUsage::TransferSrc;

		mMipData[arrayIndex][mipIndex].pStageBuffer = mDevice->CreateBuffer(bufferDesc);

		BufferDataUpdateDesc updateDesc = {};
		updateDesc.Memory = buffer;
		updateDesc.OffsetInBytes = 0;
		mDevice->UpdateHostBuffer(mMipData[arrayIndex][mipIndex].pStageBuffer.get(), updateDesc);

		mCmdBuffer->BeginRecording();
		BufferTextureCopyDesc copyDesc = {};
		copyDesc.BufferOffsetInBytes = 0;
		copyDesc.TargetArrayIndex = arrayIndex;
		copyDesc.TargetMipIndex = mipIndex;
		copyDesc.TextureSize = mImage->GetImageSize();
		copyDesc.TextureOffset = { 0, 0, 0 };
		mCmdBuffer->CopyBufferToTexture(mMipData[arrayIndex][mipIndex].pStageBuffer.get(), mImage.get(), copyDesc);
		mCmdBuffer->EndRecording();

		PipelineStageFlags waitStage = PipelineStageFlags::Transfer;
		mDevice->SubmitQueue(GraphicsAPI::GetAPI()->GetGraphicsQueue(), mCmdBuffer.get(), 1, nullptr, 0, nullptr, 0, mFence.get(), &waitStage);
		mDevice->WaitFence(mFence.get());
		mDevice->ResetFence(mFence.get());
	}

	void TextureResource::UpdateBarrier(TextureImageBarrier barrier)
	{
		mCmdBuffer->BeginRecording();
		mCmdBuffer->SetTextureBarrier(mImage.get(), barrier);
		mCmdBuffer->EndRecording();

		PipelineStageFlags waitStage = PipelineStageFlags::ColorAttachmentOutput;
		mDevice->SubmitQueue(GraphicsAPI::GetAPI()->GetGraphicsQueue(), mCmdBuffer.get(), 1, nullptr, 0, nullptr, 0, mFence.get(), &waitStage);
		mDevice->WaitFence(mFence.get());
		mDevice->ResetFence(mFence.get());
	}

	TextureView* TextureResource::GetView(u32 arrayIndex, u32 mipIndex)
	{
		if (TextureView* view = mMipData[arrayIndex][mipIndex].pView.get())
			return view;

		CORE_LOG(M_ERROR, "TextureResource", "Failed to get view for requested mipmap... Returning nullptr!!!");
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
