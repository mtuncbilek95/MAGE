#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Memory/MemoryBuffer.h"
#include "Engine/Graphics/Object/DeviceObject.h"
#include "Engine/Graphics/Command/CmdBufferDesc.h"
#include "Engine/Graphics/Command/Core/DynamicPassDesc.h"
#include "Engine/Graphics/Command/Core/GraphicsIndexType.h"
#include "Engine/Graphics/Pipeline/Core/ViewportDesc.h"
#include "Engine/Graphics/Pipeline/Core/ScissorDesc.h"
#include "Engine/Graphics/Shader/ShaderStage.h"

#include "Engine/Graphics/Command/Core/BufferBufferCopyDesc.h"
#include "Engine/Graphics/Command/Core/BufferTextureCopyDesc.h"
#include "Engine/Graphics/Command/Core/TextureImageBarrier.h"
#include "Engine/Graphics/Command/Core/BufferMemoryBarrier.h"

namespace MAGE
{
	class GraphicsBuffer;
	class Pipeline;
	class DescriptorSet;
	class TextureImage;
	class CmdPool;

	/**
	 * @class CmdBuffer
	 * @brief Base class for all command buffers
	 */
	class CmdBuffer : public DeviceObject
	{
	public:
		CmdBuffer(const CmdBufferDesc& desc, GraphicsDevice* pDevice) : DeviceObject(pDevice) , mOwnerPool(desc.pOwnerPool), mRecording(false) 
		{}
		virtual ~CmdBuffer() override = default;

		void BeginRecording();
		void EndRecording();
		void BeginRendering(const DynamicPassDesc& desc);
		void EndRendering();
		void BindPipeline(Pipeline* pPipeline);
		void BindVertexBuffers(GraphicsBuffer* pBuffer[], u32 amount);
		void BindIndexBuffer(GraphicsBuffer* pBuffer, GraphicsIndexType indexType);
		void BindDescriptors(DescriptorSet* ppSet[], u32 amount, u32 setIndex);
		void PushConstants(MemoryBuffer buffer, u32 offset, ShaderStage stage);
		void DrawIndexed(u32 indexCount, u32 indexOffset, u32 vertexOffset, u32 instanceOffset, u32 instanceCount);
		void SetViewports(ViewportDesc pViewports[], u8 count);
		void SetScissors(ScissorDesc pScissors[], u8 count);

		void CopyBufferToBuffer(GraphicsBuffer* pSourceBuffer, GraphicsBuffer* pDestinationBuffer, BufferBufferCopyDesc& desc);
		void CopyBufferToTexture(GraphicsBuffer* pSourceBuffer, TextureImage* pDestinationTexture, BufferTextureCopyDesc& desc);
		void SetTextureBarrier(TextureImage* pTexture, TextureImageBarrier& desc);
		void SetBufferBarrier(GraphicsBuffer* pBuffer, BufferMemoryBarrier& desc);

		void ExecuteCachedCommand(CmdBuffer* cmdBuffers[], u32 count);

		CmdPool* GetOwnerPool() const noexcept { return mOwnerPool; }
		GraphicsBuffer* GetBoundIndexBuffer() const noexcept { return mBoundIndexBuffer; }
		Pipeline* GetBoundPipeline() const noexcept { return mBoundPipeline; }
		b8 IsRecording() const noexcept { return mRecording; }

	protected:
		virtual void BeginRecordingImpl() = 0;
		virtual void EndRecordingImpl() = 0;
		virtual void BeginRenderingImpl(const DynamicPassDesc& desc) = 0;
		virtual void EndRenderingImpl() = 0;
		virtual void BindPipelineImpl(Pipeline* pPipeline) = 0;
		virtual void BindVertexBuffersImpl(GraphicsBuffer* ppBuffer[], u32 amount) = 0;
		virtual void BindIndexBufferImpl(GraphicsBuffer* pBuffer, GraphicsIndexType indexType) = 0;
		virtual void BindDescriptorsImpl(DescriptorSet* ppSet[], u32 amount, u32 setIndex) = 0;
		virtual void PushConstantsImpl(MemoryBuffer buffer, u32 offset, ShaderStage stage) = 0;
		virtual void DrawIndexedImpl(u32 indexCount, u32 indexOffset, u32 vertexOffset, u32 instanceOffset, u32 instanceCount) = 0;
		virtual void SetViewportsImpl(ViewportDesc pViewports[], u8 count) = 0;
		virtual void SetScissorsImpl(ScissorDesc pScissors[], u8 count) = 0;

		virtual void CopyBufferToBufferImpl(GraphicsBuffer* pSourceBuffer, GraphicsBuffer* pDestinationBuffer, BufferBufferCopyDesc& desc) = 0;
		virtual void CopyBufferToTextureImpl(GraphicsBuffer* pSourceBuffer, TextureImage* pDestinationTexture, BufferTextureCopyDesc& desc) = 0;
		virtual void SetTextureBarrierImpl(TextureImage* pTexture, TextureImageBarrier& desc) = 0;
		virtual void SetBufferBarrierImpl(GraphicsBuffer* pBuffer, BufferMemoryBarrier& desc) = 0;

		virtual void ExecuteCachedCommandImpl(CmdBuffer* cmdBuffers[], u32 count) = 0;

	private:
		CmdPool* mOwnerPool;
		GraphicsBuffer* mBoundIndexBuffer;
		Pipeline* mBoundPipeline;
		b8 mRecording;
	};
}