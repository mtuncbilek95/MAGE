#pragma once

#include "Engine/Core/Core.h"

#define MAGE_ASSERT(condition, title, message, ...) \
	if (!(condition)) \
	{ \
		MAGE::PlatformErrorMessage::ShowAssert(title, message, ##__VA_ARGS__); \
	}

namespace MAGE
{
	struct PlatformErrorMessage
	{
		static const String GetLastKnownError();
		static void ShowAssert(const String& title, const String& message, ...);
	};
}