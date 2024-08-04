#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Memory/MemoryOwnedBuffer.h>

namespace MAGE
{
	namespace ShaderCompiler
	{
		static MemoryOwnedBuffer CompileGLSLToSPIRV(const String& Path, const String& Entry = "main");
	}
}
