#pragma once

#include "Engine/Core/Core.h"
#include "Resource/Core/IResource.h"
#include "Resource/Core/ResourceSubObject.h"

namespace MAGE
{
	class RESOURCE_API ResourceObject : public IResource
	{
	public:
		static SharedPtr<ResourceObject> CreateObject();

	public:
		ResourceObject() = default;
		virtual ~ResourceObject() = default;

		FORCEINLINE const String& GetResourceName() const { return mResourceName; }
		FORCEINLINE const String& GetResourcePath() const { return mResourcePath; }
		FORCEINLINE u64 GetResourceSize() const { return mResourceSize; }
		FORCEINLINE ResourceSubObject* GetDataObject() const { return mDataObject.get(); }

	private:
		SharedPtr<ResourceSubObject> mDataObject;
		String mResourceName;
		String mResourcePath;
		u64 mResourceSize;
	};
}
