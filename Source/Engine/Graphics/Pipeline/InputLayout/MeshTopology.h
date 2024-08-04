#pragma once

#include <Engine/Core/Core.h>

namespace MAGE
{
	enum class MeshTopology : u8
	{
		PointList,
		LineList,
		LineStrip,
		TriangleList,
		PatchList,
		TriangleStrip,
		TriangleFan
	};
}