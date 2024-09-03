#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Object/DeviceObject.h"
#include "Engine/Graphics/Descriptor/DescriptorSetDesc.h"

namespace MAGE
{
	/**
	 * @class DescriptorSet
	 * @brief A descriptor set
	 */
	class DescriptorSet : public DeviceObject
	{
	public:
		DescriptorSet(const DescriptorSetDesc& desc,GraphicsDevice* pDevice) : DeviceObject(pDevice)
		{}
		virtual ~DescriptorSet() override = default;

	private:
		DescriptorPool* mOwnerPool;
		DescriptorLayout* mLayout;
	};
}