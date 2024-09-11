#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Object/IObject.h"

namespace MAGE
{
	class GraphicsDevice;

	/**
	 * @class ENGINE_API DeviceObject
	 * @brief Root class ENGINE_API for all sub graphics objects such as buffers, textures, etc.
	 */
	class ENGINE_API DeviceObject : public IObject
	{
	public:
		DeviceObject(GraphicsDevice* pDevice) : mDevice(pDevice) {}
		virtual ~DeviceObject() override = default;

		GraphicsDevice* GetOwnerDevice() const { return mDevice; }

	private:
		GraphicsDevice* mDevice;
	};
}
