#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	struct PlatformErrorMessage
	{
		static const String GetLastKnownError();
		static void ShowAssert(const String& title, const char* message, ...);
	};

	namespace Helpers
	{
		template<typename...Args>
		inline constexpr void MageAssert(bool condition, const String& title, const String& message, Args&&...args)
		{
			if (!condition)
				MAGE::PlatformErrorMessage::ShowAssert(title, message.data(), std::forward<Args>(args)...);
		}
	}
}