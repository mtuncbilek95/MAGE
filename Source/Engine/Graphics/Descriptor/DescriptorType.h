#pragma once

namespace MAGE
{
	/**
	 * @enum DescriptorType
	 * @brief Enum for the different types of descriptors
	 */
	enum class DescriptorType : u8
	{
		DT_Sampler,
		DT_CombinedImageSampler,
		DT_SampledImage,
		DT_StorageImage,
		DT_UniformTexelBuffer,
		DT_StorageTexelBuffer,
		DT_UniformBuffer,
		DT_StorageBuffer,
		DT_UniformBufferDynamic,
		DT_StorageBufferDynamic,
		DT_InputAttachment
	};
}