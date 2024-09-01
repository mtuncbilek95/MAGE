#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
    enum class PipelineStageFlags : u32
    {
        PSF_TopOfPipe = 1 << 0,
        PSF_VertexInput = 1 << 1,
        PSF_VertexShader = 1 << 2,
        PSF_TessellationControlShader = 1 << 3,
        PSF_TessellationEvaluationShader = 1 << 4,
        PSF_GeometryShader = 1 << 5,
        PSF_FragmentShader = 1 << 6,
        PSF_EarlyFragmentTests = 1 << 7,
        PSF_LateFragmentTests = 1 << 8,
        PSF_ColorAttachmentOutput = 1 << 9,
        PSF_ComputeShader = 1 << 10,
        PSF_Transfer = 1 << 11,
        PSF_BottomOfPipe = 1 << 12,
		PSF_Host = 1 << 13,
		PSF_AllGraphics = 1 << 14,
		PSF_AllCommands = 1 << 15
    };

    GENERATE_ENUM_FLAG(PipelineStageFlags, u32);
}