#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	/**
	 * @enum AttachmentStoreOperation
	 * @brief Enum for attachment store operation
	 */
	enum class AttachmentStoreOperation : u8
	{
		ASO_Store,
		ASO_DontCare,
		ASO_None
	};
}
