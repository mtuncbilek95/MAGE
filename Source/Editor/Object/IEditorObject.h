#pragma once

#include <Engine/Core/Core.h>

#include <Engine/Platform/PlatformGuid.h>
#include <Engine/Core/Guid.h>

namespace MAGE
{
	// Interface for all gui window objects
	class IEditorObject : public std::enable_shared_from_this<IEditorObject>
	{
	public:
		IEditorObject() { PlatformGuid::GenerateGuid(mGuid); }
		virtual ~IEditorObject() = default;

		template<typename T>
		FORCEINLINE SharedPtr<T> GetSharedPtrAs() { return std::static_pointer_cast<T>(shared_from_this()); }
		FORCEINLINE SharedPtr<IEditorObject> GetSharedPtr() { return shared_from_this(); }

		FORCEINLINE Guid GetGuid() const { return mGuid; }

		virtual void Shutdown() {};

	private:
		Guid mGuid;
	};
}
