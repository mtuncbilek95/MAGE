#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	/**
	 * @enum AttachmentLoadOperation
	 * @brief Enum for attachment load operation
	 */
	enum class AttachmentLoadOperation : u8
	{
		ALO_Load,
		ALO_Clear,
		ALO_DontCare
	};
}
