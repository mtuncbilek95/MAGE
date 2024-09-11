#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Object/API.h"

#include "Engine/Graphics/Instance/GraphicsInstanceDesc.h"
#include "Engine/Graphics/Swapchain/SwapchainDesc.h"

namespace MAGE
{
	class GraphicsInstance;
	class GraphicsDevice;
	class GraphicsQueue;
	class GraphicsMemory;
	class Swapchain;

	/**
	 * @class ENGINE_API GraphicsAPI
	 * @brief Graphics API class ENGINE_API that manages the graphics instance, device, queues, memory, and swapchain.
	 */
	class ENGINE_API GraphicsAPI : public API<GraphicsAPI>
	{
		friend class GraphicsModule;
	public:
		~GraphicsAPI() override = default;

		GraphicsInstance* GetInstance() const { return mInstance.get(); }
		GraphicsDevice* GetDevice() const { return mDevice.get(); }

		GraphicsQueue* GetGraphicsQueue() const { return mGraphicsQueue.get(); }
		GraphicsQueue* GetComputeQueue() const { return mComputeQueue.get(); }
		GraphicsQueue* GetTransferQueue() const { return mTransferQueue.get(); }

		GraphicsMemory* GetAvailableDeviceMemory(u64 requestedSize);
		GraphicsMemory* GetAvailableHostMemory(u64 requestedSize);

		Swapchain* GetSwapchain() const { return mSwapchain.get(); }

	private:
		SharedPtr<GraphicsInstance> CreateInstance(const GraphicsInstanceDesc& desc);
		SharedPtr<GraphicsDevice> CreateDevice();
		SharedPtr<Swapchain> CreateSwapchain(const SwapchainDesc& desc);

	private:
		SharedPtr<GraphicsInstance> mInstance;
		SharedPtr<GraphicsDevice> mDevice;
		SharedPtr<Swapchain> mSwapchain;

		SharedPtr<GraphicsQueue> mGraphicsQueue;
		SharedPtr<GraphicsQueue> mComputeQueue;
		SharedPtr<GraphicsQueue> mTransferQueue;

		List<SharedPtr<GraphicsMemory>> mDeviceMemChunks;
		List<SharedPtr<GraphicsMemory>> mHostMemChunks;
	};
}
