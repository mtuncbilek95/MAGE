#pragma once

#include <Engine/Core/Core.h>

namespace MAGE
{
	class ResourceSubObject
	{
		friend class ResourceObject;
	public:
		ResourceSubObject() = default;
		virtual ~ResourceSubObject() = default;

		class ResourceObject* GetResourceObject() const { return mResourceObject; }

	private:
		void SetResourceOwner(ResourceObject* resourceObject) { mResourceObject = resourceObject; }

	private:
		ResourceObject* mResourceObject;
	};
}
