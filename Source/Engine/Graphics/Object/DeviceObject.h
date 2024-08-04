#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Object/IObject.h>

namespace MAGE
{
	class GraphicsDevice;
	class DeviceObject : public IObject
	{
	public:
		DeviceObject(GraphicsDevice* pDevice) : mDevice(pDevice) {}
		virtual ~DeviceObject() override = default;

		GraphicsDevice* GetOwnerDevice() const { return mDevice; }

	private:
		GraphicsDevice* mDevice;
	};
}
