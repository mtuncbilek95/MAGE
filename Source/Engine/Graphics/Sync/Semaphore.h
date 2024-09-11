#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Object/DeviceObject.h"

namespace MAGE
{
	class GraphicsDevice;
	class ENGINE_API Semaphore : public DeviceObject
	{
	public:
		Semaphore(GraphicsDevice* pDevice) : DeviceObject(pDevice){}
		~Semaphore() override = default;
	};
}
