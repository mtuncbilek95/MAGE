#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Object/DeviceObject.h"
#include "Engine/Graphics/Command/CmdPoolDesc.h"

namespace MAGE
{
	/**
	 * @class ENGINE_API CmdPool
	 * @brief Abstract class ENGINE_API for command pool that holds command 
	 * buffers with a specific operation type
	 */
	class ENGINE_API CmdPool : public DeviceObject
	{
	public:
		CmdPool(const CmdPoolDesc& desc,GraphicsDevice* pDevice) : DeviceObject(pDevice), mPoolType(desc.PoolType) {}
		virtual ~CmdPool() override = default;

		CmdPoolType GetPoolType() const noexcept { return mPoolType; }

	private:
		CmdPoolType mPoolType;
	};
}