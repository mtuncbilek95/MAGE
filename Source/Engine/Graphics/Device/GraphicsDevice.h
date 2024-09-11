#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Object/IObject.h"

#include "Engine/Graphics/Queue/GraphicsQueueDesc.h"
#include "Engine/Graphics/Memory/GraphicsMemoryDesc.h"
#include "Engine/Graphics/Texture/TextureImageDesc.h"
#include "Engine/Graphics/Texture/TextureViewDesc.h"
#include "Engine/Graphics/Sampler/SamplerDesc.h"
#include "Engine/Graphics/Buffer/GraphicsBufferDesc.h"
#include "Engine/Graphics/Shader/ShaderDesc.h"
#include "Engine/Graphics/Swapchain/SwapchainDesc.h"
#include "Engine/Graphics/Descriptor/DescriptorLayoutDesc.h"
#include "Engine/Graphics/Descriptor/DescriptorPoolDesc.h"
#include "Engine/Graphics/Descriptor/DescriptorSetDesc.h"
#include "Engine/Graphics/Pipeline/GraphicsPipelineDesc.h"
#include "Engine/Graphics/Command/CmdPoolDesc.h"
#include "Engine/Graphics/Command/CmdBufferDesc.h"

#include "Engine/Graphics/Pipeline/PipelineStageFlags.h"
#include "Engine/Graphics/Core/BufferDataUpdateDesc.h"
#include "Engine/Graphics/Descriptor/DescriptorUpdateDesc.h"

namespace MAGE
{
	class GraphicsQueue;
	class GraphicsMemory;
	class TextureImage;
	class TextureView;
	class Sampler;
	class GraphicsBuffer;
	class Shader;
	class Swapchain;
	class DescriptorLayout;
	class DescriptorPool;
	class DescriptorSet;
	class Pipeline;
	class Fence;
	class Semaphore;
	class CmdPool;
	class CmdBuffer;

	class GraphicsInstance;

	/**
	 * @class ENGINE_API GraphicsDevice
	 * @brief A graphics device that handles all graphics related objects
	 */
	class ENGINE_API GraphicsDevice : public IObject
	{
	public:
		GraphicsDevice(GraphicsInstance* pInstance);
		virtual ~GraphicsDevice() override = default;

		GraphicsInstance* GetInstance() const { return mInstance; }

	public:
		SharedPtr<GraphicsQueue> CreateQueue(const GraphicsQueueDesc& desc);
		SharedPtr<GraphicsMemory> CreateMemory(const GraphicsMemoryDesc& desc);
		SharedPtr<TextureImage> CreateTextureImage(const TextureImageDesc& desc);
		SharedPtr<TextureView> CreateTextureView(const TextureViewDesc& desc);
		SharedPtr<Sampler> CreateSampler(const SamplerDesc& desc);
		SharedPtr<GraphicsBuffer> CreateBuffer(const GraphicsBufferDesc& desc);
		SharedPtr<Shader> CreateShader(const ShaderDesc& desc);
		SharedPtr<Swapchain> CreateSwapchain(const SwapchainDesc& desc);
		SharedPtr<DescriptorLayout> CreateDescriptorLayout(const DescriptorLayoutDesc& desc);
		SharedPtr<DescriptorPool> CreateDescriptorPool(const DescriptorPoolDesc& desc);
		SharedPtr<DescriptorSet> CreateDescriptorSet(const DescriptorSetDesc& desc);
		SharedPtr<Pipeline> CreateGraphicsPipeline(const GraphicsPipelineDesc& desc);
		SharedPtr<Fence> CreateGraphicsFence(b8 bSignalled);
		SharedPtr<Semaphore> CreateGraphicsSemaphore();
		SharedPtr<CmdPool> CreateCommandPool(const CmdPoolDesc& desc);
		SharedPtr<CmdBuffer> CreateCommandBuffer(const CmdBufferDesc& desc);

		void WaitFences(Fence* pFences[], u32 count);
		void WaitFence(Fence* pFence);
		void ResetFences(Fence* pFences[], u32 count);
		void ResetFence(Fence* pFence);

		void WaitIdle();
		void WaitQueueIdle(GraphicsQueue* pQueue);
		void SubmitQueue(GraphicsQueue* pQueue, CmdBuffer* pCmdBuffer, u32 cmdCount,
			Semaphore* pWaitSemaphores[], u32 waitCount, Semaphore* pSignalSemaphores[], 
			u32 signalCount, Fence* pFence, PipelineStageFlags flags[]);

		void UpdateHostBuffer(GraphicsBuffer* pBuffer, const BufferDataUpdateDesc& desc);
		void UpdateDescriptorSet(DescriptorSet* pDescriptorSet, const DescriptorUpdateDesc& desc);


	protected:
		virtual SharedPtr<GraphicsQueue> CreateQueueImpl(const GraphicsQueueDesc& desc) = 0;
		virtual SharedPtr<GraphicsMemory> CreateMemoryImpl(const GraphicsMemoryDesc& desc) = 0;
		virtual SharedPtr<TextureImage> CreateTextureImageImpl(const TextureImageDesc& desc) = 0;
		virtual SharedPtr<TextureView> CreateTextureViewImpl(const TextureViewDesc& desc) = 0;
		virtual SharedPtr<Sampler> CreateSamplerImpl(const SamplerDesc& desc) = 0;
		virtual SharedPtr<GraphicsBuffer> CreateBufferImpl(const GraphicsBufferDesc& desc) = 0;
		virtual SharedPtr<Shader> CreateShaderImpl(const ShaderDesc& desc) = 0;
		virtual SharedPtr<Swapchain> CreateSwapchainImpl(const SwapchainDesc& desc) = 0;
		virtual SharedPtr<DescriptorLayout> CreateDescriptorLayoutImpl(const DescriptorLayoutDesc& desc) = 0;
		virtual SharedPtr<DescriptorPool> CreateDescriptorPoolImpl(const DescriptorPoolDesc& desc) = 0;
		virtual SharedPtr<DescriptorSet> CreateDescriptorSetImpl(const DescriptorSetDesc& desc) = 0;
		virtual SharedPtr<Pipeline> CreateGraphicsPipelineImpl(const GraphicsPipelineDesc& desc) = 0;
		virtual SharedPtr<Fence> CreateGraphicsFenceImpl(b8 bSignalled) = 0;
		virtual SharedPtr<Semaphore> CreateGraphicsSemaphoreImpl() = 0;
		virtual SharedPtr<CmdPool> CreateCommandPoolImpl(const CmdPoolDesc& desc) = 0;
		virtual SharedPtr<CmdBuffer> CreateCommandBufferImpl(const CmdBufferDesc& desc) = 0;

		virtual void WaitFencesImpl(Fence* pFences[], u32 count) = 0;
		virtual void WaitFenceImpl(Fence* pFence) = 0;
		virtual void ResetFencesImpl(Fence* pFences[], u32 count) = 0;
		virtual void ResetFenceImpl(Fence* pFence) = 0;

		virtual void WaitIdleImpl() = 0;
		virtual void WaitQueueIdleImpl(GraphicsQueue* pQueue) = 0;
		virtual void SubmitQueueImpl(GraphicsQueue* pQueue, CmdBuffer* pCmdBuffer, u32 cmdCount,
			Semaphore* pWaitSemaphores[], u32 waitCount, Semaphore* pSignalSemaphores[], 
			u32 signalCount, Fence* pFence, PipelineStageFlags flags[]) = 0;

		virtual void UpdateHostBufferImpl(GraphicsBuffer* pBuffer, const BufferDataUpdateDesc& desc) = 0;
		virtual void UpdateDescriptorSetImpl(DescriptorSet* pDescriptorSet, const DescriptorUpdateDesc& desc) = 0;

	private:
		GraphicsInstance* mInstance;
	};
}
