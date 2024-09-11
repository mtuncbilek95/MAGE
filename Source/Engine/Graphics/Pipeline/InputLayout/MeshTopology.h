#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	enum class ENGINE_API MeshTopology : u8
	{
		MT_PointList,
		MT_LineList,
		MT_LineStrip,
		MT_TriangleList,
		MT_PatchList,
		MT_TriangleStrip,
		MT_TriangleFan
	};
}