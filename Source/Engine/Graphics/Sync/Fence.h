#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Object/DeviceObject.h>

namespace MAGE
{
	class GraphicsDevice;
	class Fence : public DeviceObject
	{
	public:
		Fence(bool bSignalled, GraphicsDevice* pDevice) : DeviceObject(pDevice), mSignalled(bSignalled) {}
		~Fence() override = default;

		bool IsSignalled() const { return mSignalled; }

	private:
		bool mSignalled;
	};
}
