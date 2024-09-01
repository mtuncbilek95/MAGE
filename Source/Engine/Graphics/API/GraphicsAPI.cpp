#include "GraphicsAPI.h"

#include "Engine/Vulkan/Instance/VInstance.h"
#include "Engine/Vulkan/Device/VDevice.h"

#include "Engine/Graphics/Queue/GraphicsQueue.h"
#include "Engine/Graphics/Memory/GraphicsMemory.h"

namespace MAGE
{
	SharedPtr<GraphicsInstance> GraphicsAPI::CreateInstance(const GraphicsInstanceDesc& desc)
	{
		mInstance = MakeShared<VInstance>(desc);
		return mInstance;
	}

	SharedPtr<GraphicsDevice> GraphicsAPI::CreateDevice()
	{
		mDevice = MakeShared<VDevice>(mInstance->GetAs<VInstance>());

		mGraphicsQueue = mDevice->CreateQueue({ GraphicsQueueType::GQT_Graphics, mDevice->GetAs<VDevice>()->GetGraphicsQueueFamilyIndex() });
		mComputeQueue = mDevice->CreateQueue({ GraphicsQueueType::GQT_Compute, mDevice->GetAs<VDevice>()->GetComputeQueueFamilyIndex() });
		mTransferQueue = mDevice->CreateQueue({ GraphicsQueueType::GQT_Transfer, mDevice->GetAs<VDevice>()->GetTransferQueueFamilyIndex() });

		return mDevice;
	}

	GraphicsMemory* GraphicsAPI::GetAvailableDeviceMemory(u64 requestedSize)
	{
		// Check all the memories in the chunk and return the first one that has enough space
		for (auto& memory : mDeviceMemChunks)
			if (memory->GetFreeMemorySize() >= requestedSize)
				return memory.get();

		// If no memory chunk has enough space, create a new one
		u64 newSize = requestedSize > Helpers::MbToByte(500.0) ? requestedSize : Helpers::MbToByte(500.0);
		SharedPtr<GraphicsMemory> newMemory = mDevice->CreateMemory({ GraphicsMemoryType::GMT_DeviceLocal, newSize });
		mDeviceMemChunks.push_back(newMemory);
		return newMemory.get();
	}

	GraphicsMemory* GraphicsAPI::GetAvailableHostMemory(u64 requestedSize)
	{
		// Check all the memories in the chunk and return the first one that has enough space
		for (auto& memory : mHostMemChunks)
			if (memory->GetFreeMemorySize() >= requestedSize)
				return memory.get();

		// If no memory chunk has enough space, create a new one
		u64 newSize = requestedSize > Helpers::MbToByte(500.0) ? requestedSize : Helpers::MbToByte(500.0);
		SharedPtr<GraphicsMemory> newMemory = mDevice->CreateMemory({ GraphicsMemoryType::GMT_HostVisible, newSize });
		mHostMemChunks.push_back(newMemory);
		return newMemory.get();
	}

	SharedPtr<Swapchain> GraphicsAPI::CreateSwapchain(const SwapchainDesc& desc)
	{
		mSwapchain = mDevice->CreateSwapchain(desc);
		return mSwapchain;
	}
}
