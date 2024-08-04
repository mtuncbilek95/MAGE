#include "GraphicsDevice.h"

namespace MAGE
{
	GraphicsDevice::GraphicsDevice(GraphicsInstance* pInstance) : mInstance(pInstance)
	{
	}

	SharedPtr<GraphicsQueue> GraphicsDevice::CreateQueue(const GraphicsQueueDesc& desc)
	{
		return CreateQueueImpl(desc);
	}

	SharedPtr<GraphicsMemory> GraphicsDevice::CreateMemory(const GraphicsMemoryDesc& desc)
	{
		return CreateMemoryImpl(desc);
	}

	SharedPtr<TextureImage> GraphicsDevice::CreateTextureImage(const TextureImageDesc& desc)
	{
		return CreateTextureImageImpl(desc);
	}

	SharedPtr<TextureView> GraphicsDevice::CreateTextureView(const TextureViewDesc& desc)
	{
		return CreateTextureViewImpl(desc);
	}

	SharedPtr<Sampler> GraphicsDevice::CreateSampler(const SamplerDesc& desc)
	{
		return CreateSamplerImpl(desc);
	}

	SharedPtr<GraphicsBuffer> GraphicsDevice::CreateBuffer(const GraphicsBufferDesc& desc)
	{
		return CreateBufferImpl(desc);
	}

	SharedPtr<Shader> GraphicsDevice::CreateShader(const ShaderDesc& desc)
	{
		return CreateShaderImpl(desc);
	}

	SharedPtr<Swapchain> GraphicsDevice::CreateSwapchain(const SwapchainDesc& desc)
	{
		return CreateSwapchainImpl(desc);
	}

	SharedPtr<DescriptorLayout> GraphicsDevice::CreateDescriptorLayout(const DescriptorLayoutDesc& desc)
	{
		return CreateDescriptorLayoutImpl(desc);
	}

	SharedPtr<DescriptorPool> GraphicsDevice::CreateDescriptorPool(const DescriptorPoolDesc& desc)
	{
		return CreateDescriptorPoolImpl(desc);
	}

	SharedPtr<DescriptorSet> GraphicsDevice::CreateDescriptorSet(const DescriptorSetDesc& desc)
	{
		return CreateDescriptorSetImpl(desc);
	}

	SharedPtr<Pipeline> GraphicsDevice::CreateGraphicsPipeline(const GraphicsPipelineDesc& desc)
	{
		return CreateGraphicsPipelineImpl(desc);
	}

	SharedPtr<Fence> GraphicsDevice::CreateGraphicsFence(bool bSignalled)
	{
		return CreateGraphicsFenceImpl(bSignalled);
	}

	SharedPtr<Semaphore> GraphicsDevice::CreateGraphicsSemaphore()
	{
		return CreateGraphicsSemaphoreImpl();
	}

	SharedPtr<CmdPool> GraphicsDevice::CreateCommandPool(const CmdPoolDesc& desc)
	{
		return CreateCommandPoolImpl(desc);
	}

	SharedPtr<CmdBuffer> GraphicsDevice::CreateCommandBuffer(const CmdBufferDesc& desc)
	{
		return CreateCommandBufferImpl(desc);
	}

	void GraphicsDevice::WaitFences(Fence* pFences[], u32 count)
	{
		WaitFencesImpl(pFences, count);
	}

	void GraphicsDevice::WaitFence(Fence* pFence)
	{
		WaitFenceImpl(pFence);
	}

	void GraphicsDevice::ResetFences(Fence* pFences[], u32 count)
	{
		ResetFencesImpl(pFences, count);
	}

	void GraphicsDevice::ResetFence(Fence* pFence)
	{
		ResetFenceImpl(pFence);
	}

	void GraphicsDevice::WaitIdle()
	{
		WaitIdleImpl();
	}

	void GraphicsDevice::WaitQueueIdle(GraphicsQueue* pQueue)
	{
		WaitQueueIdleImpl(pQueue);
	}

	void GraphicsDevice::SubmitQueue(GraphicsQueue* pQueue, CmdBuffer* pCmdBuffer, u32 cmdCount, Semaphore* pWaitSemaphores[], u32 waitCount, Semaphore* pSignalSemaphores[], u32 signalCount, Fence* pFence, PipelineStageFlags flags[])
	{
		SubmitQueueImpl(pQueue, pCmdBuffer, cmdCount, pWaitSemaphores, waitCount, pSignalSemaphores, signalCount, pFence, flags);
	}

	void GraphicsDevice::UpdateHostBuffer(GraphicsBuffer* pBuffer, const BufferDataUpdateDesc& desc)
	{
		UpdateHostBufferImpl(pBuffer, desc);
	}

	void GraphicsDevice::UpdateDescriptorSet(DescriptorSet* pDescriptorSet, const DescriptorUpdateDesc& desc)
	{
		UpdateDescriptorSetImpl(pDescriptorSet, desc);
	}
}
