#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Object/DeviceObject.h"
#include "Engine/Graphics/Queue/GraphicsQueueDesc.h"

namespace MAGE
{
	class GraphicsQueue : public DeviceObject
	{
	public:
		GraphicsQueue(const GraphicsQueueDesc& desc, GraphicsDevice* pDevice) : DeviceObject(pDevice),
			mQueueType(desc.QueueType), mQueueIndex(desc.QueueIndex) {}
		virtual ~GraphicsQueue() override = default;

		GraphicsQueueType GetQueueType() const { return mQueueType; }
		u32 GetQueueIndex() const { return mQueueIndex; }
	private:
		GraphicsQueueType mQueueType;
		u32 mQueueIndex;
	};
}
