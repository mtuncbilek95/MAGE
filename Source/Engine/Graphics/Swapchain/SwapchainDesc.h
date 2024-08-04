#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Graphics/Texture/TextureFormat.h>
#include <Engine/Graphics/Texture/TextureUsageFlags.h>
#include <Engine/Graphics/Texture/TextureType.h>
#include <Engine/Graphics/Swapchain/PresentMode.h>

namespace MAGE
{
	class GraphicsQueue;
	struct SwapchainDesc
	{
		Vec2u ImageSize;
		TextureFormat ImageFormat;
		TextureUsageFlags TextureUsage;
		PresentMode VSync;
		u8 BufferCount;
		GraphicsQueue* pRequestQueue;
	};
}