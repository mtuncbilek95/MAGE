#include "DXAssert.h"

#include <spdlog/spdlog.h>

namespace Mage
{
	b8 Assertion::DXAssert(HRESULT result, const string& title)
	{
		if (FAILED(result))
		{
			spdlog::error("Please check your DebugLayer, then check {}", title);
			return false;
		}

		return true;
	}
}