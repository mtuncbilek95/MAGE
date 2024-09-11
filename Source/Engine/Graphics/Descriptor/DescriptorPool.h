#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Object/DeviceObject.h"
#include "Engine/Graphics/Descriptor/DescriptorPoolDesc.h"

namespace MAGE
{
	/**
	 * @class ENGINE_API DescriptorPool
	 * @brief A descriptor pool
	 */
	class ENGINE_API DescriptorPool : public DeviceObject
	{
	public:
		DescriptorPool(const DescriptorPoolDesc& desc, GraphicsDevice* pDevice) : DeviceObject(pDevice), mMaxSets(desc.MaxSets),
			mPoolSizes(desc.PoolSizes)
		{}
		virtual ~DescriptorPool() override = default;

		u64 GetMaxSets() const noexcept { return mMaxSets; }
		const Vector<DescriptorPoolSize>& GetPoolSizes() const noexcept { return mPoolSizes; }

	private:
		u64 mMaxSets;
		Vector<DescriptorPoolSize> mPoolSizes;
	};
}