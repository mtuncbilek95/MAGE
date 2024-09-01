#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	enum class FaceCullMode : u8
	{
		FCM_None = 1,
		FCM_Front = 2,
		FCM_Back = 4,
		FCM_All = 8
	};
}