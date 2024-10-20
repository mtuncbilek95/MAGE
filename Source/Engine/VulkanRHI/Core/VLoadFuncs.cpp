#include "VLoadFuncs.h"

PFN_vkGetDescriptorSetLayoutSizeEXT GetDescriptorSetLayoutSizeEXT = nullptr;
PFN_vkGetDescriptorSetLayoutBindingOffsetEXT GetDescriptorSetLayoutBindingOffsetEXT = nullptr;

PFN_vkCreateDebugUtilsMessengerEXT CreateDebugMessengerEXT = nullptr;
PFN_vkDestroyDebugUtilsMessengerEXT DestroyDebugMessengerEXT = nullptr;

void LoadVulkanDeviceFunctions(VkDevice device)
{
	// Get Descriptor Set Layout Size
	GetDescriptorSetLayoutSizeEXT = reinterpret_cast<PFN_vkGetDescriptorSetLayoutSizeEXT>(vkGetDeviceProcAddr(device, "vkGetDescriptorSetLayoutSizeEXT"));
	// Get Descriptor Set Layout Binding Offset
	GetDescriptorSetLayoutBindingOffsetEXT = reinterpret_cast<PFN_vkGetDescriptorSetLayoutBindingOffsetEXT>(vkGetDeviceProcAddr(device, "vkGetDescriptorSetLayoutBindingOffsetEXT"));
}

void LoadVulkanInstanceFunctions(VkInstance instance)
{
	// Create Debug Messenger
	CreateDebugMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
	// Destroy Debug Messenger
	DestroyDebugMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
}
