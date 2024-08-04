#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Object/DeviceObject.h>
#include <Engine/Graphics/Swapchain/SwapchainDesc.h>

namespace MAGE
{
	class GraphicsQueue;
	class Fence;
	class Semaphore;
	class GraphicsDevice;
	class TextureImage;
	class TextureView;
	class Swapchain : public DeviceObject
	{
	public:
		Swapchain(const SwapchainDesc& desc, GraphicsDevice* pDevice) : DeviceObject(pDevice),
			mImageSize(desc.ImageSize), mImageFormat(desc.ImageFormat), mTextureUsage(desc.TextureUsage),
			mVSync(desc.VSync), mBufferCount(desc.BufferCount), mRequestQueue(desc.pRequestQueue)
		{}
		virtual ~Swapchain() override = default;

		const Vec2u& GetImageSize() const { return mImageSize; }
		TextureFormat GetImageFormat() const { return mImageFormat; }
		TextureUsageFlags GetTextureUsage() const { return mTextureUsage; }
		PresentMode GetVSync() const { return mVSync; }
		u8 GetBufferCount() const { return mBufferCount; }
		u32 GetImageIndex() const { return mImageIndex; }


		TextureImage* GetImage(u32 index) { return mImages[index].get(); }
		TextureView* GetImageView(u32 index) const { return mImageViews[index].get(); }

		u32 AcquireNextImage(Fence* pFence, Semaphore* pSemaphore)
		{
			mImageIndex = AcquireNextImageImpl(pFence, pSemaphore);
			return mImageIndex;
		}

		void Present(Semaphore* pSemaphore)
		{
			PresentImpl(pSemaphore);
		}

		void ResizeSwapchain(Vec2u newSize)
		{
			mImageSize = newSize;
			ResizeSwapchainImpl(newSize);
		}

	protected:
		virtual u32 AcquireNextImageImpl(Fence* pFence, Semaphore* pSemaphore) = 0;
		virtual void PresentImpl(Semaphore* pSemaphore) = 0;
		virtual void ResizeSwapchainImpl(Vec2u newSize) = 0;

		void SetNewImageSize(Vec2u newSize) { mImageSize = newSize; }

		DArray<SharedPtr<TextureImage>> mImages;
		DArray<SharedPtr<TextureView>> mImageViews;
		GraphicsQueue* mRequestQueue;
		u32 mImageIndex = 0;

	private:
		Vec2u mImageSize;
		TextureFormat mImageFormat;
		TextureUsageFlags mTextureUsage;
		PresentMode mVSync;
		u8 mBufferCount;
	};
}