#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Object/DeviceObject.h"
#include "Engine/Graphics/Buffer/GraphicsBufferDesc.h"

namespace MAGE
{
	class GraphicsMemory;
	class GraphicsBuffer : public DeviceObject
	{
	public:
		GraphicsBuffer(const GraphicsBufferDesc& desc, GraphicsDevice* pDevice) : DeviceObject(pDevice),
			mUsage(desc.Usage), mBlockSize(desc.BlockSize), mBlockLength(desc.BlockLength),
			mRequestMemory(desc.pRequestMemory)
		{}
		virtual ~GraphicsBuffer() override = default;

		GraphicsBufferUsage GetBufferUsage() const { return mUsage; }
		u32 GetBlockLength() const { return mBlockLength; }
		u32 GetBlockSize() const { return mBlockSize; }
		u32 GetTotalSize() const { return mBlockSize * mBlockLength; }

		GraphicsMemory* GetMemory() const { return mRequestMemory; }

	private:
		u64 mBlockSize;
		u64 mBlockLength;
		GraphicsBufferUsage mUsage;
		GraphicsMemory* mRequestMemory;
	};
}