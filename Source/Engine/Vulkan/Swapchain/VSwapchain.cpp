#include "VSwapchain.h"
#include "VSwapchainUtils.h"
#include "../Texture/VTextureUtils.h"

#if defined(MAGE_WINDOWS)
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#endif

#if defined(MAGE_LINUX)
#include <vulkan/vulkan_xlib.h>
#endif

#include <Engine/Vulkan/Device/VDevice.h>
#include <Engine/Vulkan/Queue/VQueue.h>
#include <Engine/Vulkan/Sync/VFence.h>
#include <Engine/Vulkan/Sync/VSemaphore.h>

#include <Engine/Window/WindowAPI.h>
#include <Engine/Graphics/Texture/TextureImage.h>
#include <Engine/Graphics/Texture/TextureView.h>

#include <Engine/Graphics/API/GraphicsAPI.h>

#include <Engine/Vulkan/Texture/VTextureImage.h>

namespace MAGE
{
	VSwapchain::VSwapchain(const SwapchainDesc& desc, VDevice* pDevice) : Swapchain(desc, pDevice), mDevice(pDevice->GetVkDevice()),
		mInstance(pDevice->GetVkInstance()), mAdapter(pDevice->GetVkAdapter()), mSwapchain(VK_NULL_HANDLE), mSurface(VK_NULL_HANDLE)
	{
#if defined(MAGE_WINDOWS)
		VkWin32SurfaceCreateInfoKHR surfaceInfo = {};
		surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		surfaceInfo.hinstance = WindowAPI::GetAPI()->GetDefaultWindow()->GetInstanceHandle();
		surfaceInfo.hwnd = WindowAPI::GetAPI()->GetDefaultWindow()->GetWindowHandle();
		surfaceInfo.pNext = nullptr;

		CORE_ASSERT(vkCreateWin32SurfaceKHR(mInstance, &surfaceInfo, nullptr, &mSurface) == VK_SUCCESS, "VSwapchain", "Failed to create win32 surface");
#endif

#if defined(MAGE_LINUX)
		VkXlibSurfaceCreateInfoKHR surfaceInfo = {};
		surfaceInfo.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
		surfaceInfo.dpy = WindowAPI::GetAPI()->GetDefaultWindow()->GetDisplayHandle();
		surfaceInfo.window = WindowAPI::GetAPI()->GetDefaultWindow()->GetWindowHandle();
		surfaceInfo.pNext = nullptr;
#endif

		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		CORE_ASSERT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mAdapter, mSurface, &surfaceCapabilities) == VK_SUCCESS, "VSwapchain",
			"Failed to get surface capabilities");

		u32 formatCount;
		CORE_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(mAdapter, mSurface, &formatCount, nullptr) == VK_SUCCESS, "VSwapchain",
			"Failed to get surface formats");
		CORE_ASSERT(formatCount > 0, "VSwapchain", "No surface formats found");

		DArray<VkSurfaceFormatKHR> surfaceFormats(formatCount);
		CORE_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(mAdapter, mSurface, &formatCount, surfaceFormats.data()) == VK_SUCCESS, "VSwapchain",
			"Failed to get surface formats");

		VkColorSpaceKHR colorSpace = {};
		for (auto& format : surfaceFormats)
		{
			if (format.format == VkUtils::GetVkFormat(desc.ImageFormat) && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				colorSpace = format.colorSpace;
				break;
			}
		}

		if (surfaceCapabilities.maxImageExtent.width > 0 && surfaceCapabilities.maxImageExtent.height > 0)
		{
			if (desc.ImageSize.x > surfaceCapabilities.maxImageExtent.width || desc.ImageSize.y > surfaceCapabilities.maxImageExtent.height)
			{
				CORE_LOG(M_WARNING, "Requested image size is not supported. Using the current extent size.");
				SetNewImageSize({ surfaceCapabilities.currentExtent.width, surfaceCapabilities.currentExtent.height });
			}
		}

		if (desc.ImageSize.x == 0 || desc.ImageSize.y == 0)
			SetNewImageSize({ surfaceCapabilities.maxImageExtent.width, surfaceCapabilities.maxImageExtent.height });

		u32 presentModeCount = 0;
		CORE_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(mAdapter, mSurface, &presentModeCount, nullptr) == VK_SUCCESS,
			"CreateSurfaceSwapchain", "Failed to get present modes");
		CORE_ASSERT(presentModeCount > 0, "VSwapchan", "No present modes found");

		DArray<VkPresentModeKHR> presentModes(presentModeCount);
		CORE_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(mAdapter, mSurface, &presentModeCount, presentModes.data()) == VK_SUCCESS,
			"CreateSurfaceSwapchain", "Failed to get present modes");

		auto pVkQueue = desc.pRequestQueue->GetAs<VQueue>();
		u32 presentQueueFamilyIndex = pVkQueue->GetQueueIndex();
		CORE_ASSERT(presentQueueFamilyIndex != UINT32_MAX, "VSwapchan", "Failed to get present queue family index");
		VkBool32 presentSupport = false;
		CORE_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(mAdapter, presentQueueFamilyIndex, mSurface, &presentSupport) == VK_SUCCESS,
			"VSwapchan", "Failed to get present support");

		CORE_ASSERT(presentSupport, "VSwapchan", "Present support not found");

		// Create the swapchain
		VkSwapchainCreateInfoKHR swapchainInfo = {};
		swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainInfo.surface = mSurface;
		swapchainInfo.minImageCount = desc.BufferCount;
		swapchainInfo.imageFormat = VkUtils::GetVkFormat(desc.ImageFormat);
		swapchainInfo.imageColorSpace = colorSpace;
		swapchainInfo.imageExtent = { GetImageSize().x, GetImageSize().y };
		swapchainInfo.imageArrayLayers = 1;
		swapchainInfo.imageUsage = VkUtils::GetVkImageUsageFlags(desc.TextureUsage);
		swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainInfo.queueFamilyIndexCount = 1;
		swapchainInfo.pQueueFamilyIndices = &presentQueueFamilyIndex;
		swapchainInfo.queueFamilyIndexCount = 1;
		swapchainInfo.preTransform = surfaceCapabilities.currentTransform;
		swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		swapchainInfo.presentMode = VkUtils::GetVkPresentMode(desc.VSync);
		swapchainInfo.clipped = VK_FALSE;
		swapchainInfo.oldSwapchain = VK_NULL_HANDLE;
		swapchainInfo.flags = VkSwapchainCreateFlagsKHR();
		swapchainInfo.pNext = nullptr;

		CORE_ASSERT(vkCreateSwapchainKHR(mDevice, &swapchainInfo, nullptr, &mSwapchain) == VK_SUCCESS, "VSwapchain", "Failed to create swapchain");

		u32 imageCount = 0;
		CORE_ASSERT(vkGetSwapchainImagesKHR(mDevice, mSwapchain, &imageCount, nullptr) == VK_SUCCESS, "VSwapchan", "Failed to get swapchain images");
		CORE_ASSERT(imageCount > 0, "VSwapchan", "No swapchain images found");

		DArray<VkImage> images(imageCount);
		CORE_ASSERT(vkGetSwapchainImagesKHR(mDevice, mSwapchain, &imageCount, images.data()) == VK_SUCCESS, "VSwapchan", "Failed to get swapchain images");


		// Nevertheless, we need to fill the VulkanTexture data
		for (u32 i = 0; i < imageCount; i++)
		{
			TextureImageDesc textureDesc = {};
			textureDesc.ArrayLayers = 1;
			textureDesc.MipLevels = 1;
			textureDesc.ImageFormat = desc.ImageFormat;
			textureDesc.ImageSize = { GetImageSize().x, GetImageSize().y, 1 };
			textureDesc.Samples = SampleCount::Sample1;
			textureDesc.ImageType = TextureType::Texture2D;
			textureDesc.UsageFlags = desc.TextureUsage;

			auto pTexture = GetOwnerDevice()->GetAs<VDevice>()->CreateSwapchainImage(textureDesc, images[i]);
			mImages.push_back(pTexture);

			// After creating the VulkanTexture, we need to create the VulkanTextureView
			TextureViewDesc viewDesc = {};
			viewDesc.pTexture = pTexture.get();
			viewDesc.ArrayLayer = 0;
			viewDesc.ArrayCount = 1;
			viewDesc.MipLevel = 0;
			viewDesc.MipCount = 1;
			viewDesc.AspectFlags = TextureAspectFlags::ColorAspect;
			viewDesc.ViewType = TextureViewType::Texture2D;

			auto pTextureView = GetOwnerDevice()->GetAs<VDevice>()->CreateSwapchainImageView(viewDesc);
			mImageViews.push_back(pTextureView);
		}
	}

	VSwapchain::~VSwapchain()
	{
		for (auto& image : mImages)
			image->GetAs<VTextureImage>()->NullifyTexture();

		mImageViews.clear();
		mImageViews.shrink_to_fit();

		vkDestroySwapchainKHR(mDevice, mSwapchain, nullptr);
		vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
	}

	u32 VSwapchain::AcquireNextImageImpl(Fence* pFence, Semaphore* pSemaphore)
	{
		u32 imageIndex = 0;
		if (auto pF = pFence)
		{
			VkFence fence = pF->GetAs<VFence>()->GetVkFence();
			CORE_ASSERT(vkAcquireNextImageKHR(mDevice, mSwapchain, UINT64_MAX, VK_NULL_HANDLE, fence, &imageIndex) == VK_SUCCESS, "VSwapchain",
				"Failed to acquire next image");
		}

		if (auto pS = pSemaphore)
		{
			VkSemaphore semaphore = pS->GetAs<VSemaphore>()->GetVkSemaphore();
			CORE_ASSERT(vkAcquireNextImageKHR(mDevice, mSwapchain, UINT64_MAX, semaphore, VK_NULL_HANDLE, &imageIndex) == VK_SUCCESS, "VSwapchain",
				"Failed to acquire next image");
		}
		return imageIndex;
	}

	void VSwapchain::PresentImpl(Semaphore* pSemaphore)
	{
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.pNext = nullptr;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &mSwapchain;
		presentInfo.pImageIndices = &mImageIndex;

		if (auto pS = pSemaphore)
		{
			VkSemaphore semaphore = pS->GetAs<VSemaphore>()->GetVkSemaphore();
			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = &semaphore;
		}

		VkQueue queue = GraphicsAPI::GetAPI()->GetGraphicsQueue()->GetAs<VQueue>()->GetVkQueue();

		CORE_ASSERT(vkQueuePresentKHR(queue, &presentInfo) == VK_SUCCESS, "VSwapchain",
			"Failed to present image");
	}

	void VSwapchain::ResizeSwapchainImpl(Vec2u newSize)
	{
		if (newSize.x == 0 || newSize.y == 0)
			return;

		mImageViews.clear();
		mImageViews.shrink_to_fit();

		for (auto& image : mImages)
			image->GetAs<VTextureImage>()->NullifyTexture();

		vkDeviceWaitIdle(mDevice);

		if(mSwapchain != VK_NULL_HANDLE)
			vkDestroySwapchainKHR(mDevice, mSwapchain, nullptr);

		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		CORE_ASSERT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mAdapter, mSurface, &surfaceCapabilities) == VK_SUCCESS, "VSwapchain",
			"Failed to get surface capabilities");

		u32 formatCount;
		CORE_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(mAdapter, mSurface, &formatCount, nullptr) == VK_SUCCESS, "VSwapchain",
			"Failed to get surface formats");
		CORE_ASSERT(formatCount > 0, "VSwapchain", "No surface formats found");

		DArray<VkSurfaceFormatKHR> surfaceFormats(formatCount);
		CORE_ASSERT(vkGetPhysicalDeviceSurfaceFormatsKHR(mAdapter, mSurface, &formatCount, surfaceFormats.data()) == VK_SUCCESS, "VSwapchain",
			"Failed to get surface formats");

		VkColorSpaceKHR colorSpace = {};
		for (auto& format : surfaceFormats)
		{
			if (format.format == VkUtils::GetVkFormat(GetImageFormat()) && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				colorSpace = format.colorSpace;
				break;
			}
		}

		if (surfaceCapabilities.maxImageExtent.width > 0 && surfaceCapabilities.maxImageExtent.height > 0)
		{
			if (newSize.x > surfaceCapabilities.maxImageExtent.width || newSize.y > surfaceCapabilities.maxImageExtent.height)
			{
				CORE_LOG(M_WARNING, "Requested image size is not supported. Using the current extent size.");
				SetNewImageSize({ surfaceCapabilities.currentExtent.width, surfaceCapabilities.currentExtent.height });
			}
		}

		u32 presentModeCount = 0;
		CORE_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(mAdapter, mSurface, &presentModeCount, nullptr) == VK_SUCCESS,
			"CreateSurfaceSwapchain", "Failed to get present modes");
		CORE_ASSERT(presentModeCount > 0, "VSwapchan", "No present modes found");

		DArray<VkPresentModeKHR> presentModes(presentModeCount);
		CORE_ASSERT(vkGetPhysicalDeviceSurfacePresentModesKHR(mAdapter, mSurface, &presentModeCount, presentModes.data()) == VK_SUCCESS,
			"CreateSurfaceSwapchain", "Failed to get present modes");

		auto pVkQueue = mRequestQueue->GetAs<VQueue>();
		u32 presentQueueFamilyIndex = pVkQueue->GetQueueIndex();
		CORE_ASSERT(presentQueueFamilyIndex != UINT32_MAX, "VSwapchan", "Failed to get present queue family index");
		VkBool32 presentSupport = false;
		CORE_ASSERT(vkGetPhysicalDeviceSurfaceSupportKHR(mAdapter, presentQueueFamilyIndex, mSurface, &presentSupport) == VK_SUCCESS,
			"VSwapchan", "Failed to get present support");

		CORE_ASSERT(presentSupport, "VSwapchan", "Present support not found");

		VkSwapchainCreateInfoKHR swapchainInfo = {};
		swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainInfo.surface = mSurface;
		swapchainInfo.minImageCount = GetBufferCount();
		swapchainInfo.imageFormat = VkUtils::GetVkFormat(GetImageFormat());
		swapchainInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
		swapchainInfo.imageExtent = { GetImageSize().x, GetImageSize().y};
		swapchainInfo.imageArrayLayers = 1;
		swapchainInfo.imageUsage = VkUtils::GetVkImageUsageFlags(GetTextureUsage());
		swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainInfo.queueFamilyIndexCount = 1;
		swapchainInfo.pQueueFamilyIndices = &presentQueueFamilyIndex;
		swapchainInfo.queueFamilyIndexCount = 1;
		swapchainInfo.preTransform = surfaceCapabilities.currentTransform;
		swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		swapchainInfo.presentMode = VkUtils::GetVkPresentMode(GetVSync());
		swapchainInfo.clipped = VK_FALSE;
		swapchainInfo.oldSwapchain = VK_NULL_HANDLE;
		swapchainInfo.flags = VkSwapchainCreateFlagsKHR();
		swapchainInfo.pNext = nullptr;

		CORE_ASSERT(vkCreateSwapchainKHR(mDevice, &swapchainInfo, nullptr, &mSwapchain) == VK_SUCCESS, "VSwapchain", "Failed to create swapchain");

		u32 imageCount = 0;
		CORE_ASSERT(vkGetSwapchainImagesKHR(mDevice, mSwapchain, &imageCount, nullptr) == VK_SUCCESS, "VSwapchan", "Failed to get swapchain images");
		CORE_ASSERT(imageCount > 0, "VSwapchan", "No swapchain images found");

		DArray<VkImage> images(imageCount);
		CORE_ASSERT(vkGetSwapchainImagesKHR(mDevice, mSwapchain, &imageCount, images.data()) == VK_SUCCESS, "VSwapchan", "Failed to get swapchain images");

		// Nevertheless, we need to fill the VulkanTexture data
		for (u32 i = 0; i < imageCount; i++)
		{
			TextureImageDesc textureDesc = {};
			textureDesc.ArrayLayers = 1;
			textureDesc.MipLevels = 1;
			textureDesc.ImageFormat = GetImageFormat();
			textureDesc.ImageSize = { GetImageSize().x, GetImageSize().y, 1 };
			textureDesc.Samples = SampleCount::Sample1;
			textureDesc.ImageType = TextureType::Texture2D;
			textureDesc.UsageFlags = GetTextureUsage();

			auto pTexture = GetOwnerDevice()->GetAs<VDevice>()->CreateSwapchainImage(textureDesc, images[i]);
			mImages[i] = pTexture;

			// After creating the VulkanTexture, we need to create the VulkanTextureView
			TextureViewDesc viewDesc = {};
			viewDesc.pTexture = pTexture.get();
			viewDesc.ArrayLayer = 0;
			viewDesc.ArrayCount = 1;
			viewDesc.MipLevel = 0;
			viewDesc.MipCount = 1;
			viewDesc.AspectFlags = TextureAspectFlags::ColorAspect;
			viewDesc.ViewType = TextureViewType::Texture2D;

			auto pTextureView = GetOwnerDevice()->GetAs<VDevice>()->CreateSwapchainImageView(viewDesc);
			mImageViews.push_back(pTextureView);
		}
	}
}
