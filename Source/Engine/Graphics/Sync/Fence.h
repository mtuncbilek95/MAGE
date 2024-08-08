#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Object/DeviceObject.h>

namespace MAGE
{
	class GraphicsDevice;
	class Fence : public DeviceObject
	{
	public:
		Fence(b8 bSignalled, GraphicsDevice* pDevice) : DeviceObject(pDevice), mSignalled(bSignalled) {}
		~Fence() override = default;

		b8 IsSignalled() const { return mSignalled; }

	private:
		b8 mSignalled;
	};
}
