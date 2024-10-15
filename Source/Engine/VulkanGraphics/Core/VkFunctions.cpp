#include "VkFunctions.h"

PFN_vkGetDescriptorSetLayoutSizeEXT GetDescriptorSetLayoutSizeEXT = nullptr;
PFN_vkGetDescriptorSetLayoutBindingOffsetEXT GetDescriptorSetLayoutBindingOffsetEXT = nullptr;

void LoadVulkanExtensionFunctions(VkDevice device)
{
	// Get Descriptor Set Layout Size
	GetDescriptorSetLayoutSizeEXT = reinterpret_cast<PFN_vkGetDescriptorSetLayoutSizeEXT>(vkGetDeviceProcAddr(device, "vkGetDescriptorSetLayoutSizeEXT"));
	// Get Descriptor Set Layout Binding Offset
	GetDescriptorSetLayoutBindingOffsetEXT = reinterpret_cast<PFN_vkGetDescriptorSetLayoutBindingOffsetEXT>(vkGetDeviceProcAddr(device, "vkGetDescriptorSetLayoutBindingOffsetEXT"));
}
