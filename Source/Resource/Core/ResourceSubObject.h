#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Serialization/GenerationMacros.h"

namespace MAGE
{
	class ResourceObject;

	class RESOURCE_API ResourceSubObject
	{
		friend class ResourceObject;
	public:
		ResourceSubObject() = default;
		virtual ~ResourceSubObject() = default;

		FORCEINLINE ResourceObject* GetResourceObject() const { return mResourceObject; }

	private:
		void SetResourceOwner(ResourceObject* resourceObject) { mResourceObject = resourceObject; }

	private:
		ResourceObject* mResourceObject;
	};
}
