#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Graphics/Command/CmdPool.h"

namespace MAGE
{
	struct CmdBufferDesc
	{
		CmdPool* pOwnerPool;
	};
}