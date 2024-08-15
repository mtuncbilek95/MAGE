#pragma once

#include <Engine/Core/Core.h>

#include <meta/meta.hpp>

namespace MAGE
{
	class Type;

	/**
	 * @class Class
	 * @brief This class is the root for all reflection objects.
	 */
	class Class
	{
	public:
		// Easy getter for the class' type. It can be used to check if the class is a specific type.
		FORCEINLINE virtual Type* GetType() const noexcept { return nullptr; }
	};
}
