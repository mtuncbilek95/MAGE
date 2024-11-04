#include "VLoadFuncs.h"

PFN_vkGetDescriptorSetLayoutSizeEXT GetDescriptorSetLayoutSizeEXT = nullptr;
PFN_vkGetDescriptorSetLayoutBindingOffsetEXT GetDescriptorSetLayoutBindingOffsetEXT = nullptr;
PFN_vkGetDescriptorEXT GetDescriptorEXT = nullptr;
PFN_vkCmdBindDescriptorBuffersEXT BindDescriptorEXT = nullptr;
PFN_vkCmdSetDescriptorBufferOffsetsEXT SetDescriptorBufferOffsetEXT = nullptr;

PFN_vkCreateDebugUtilsMessengerEXT CreateDebugMessengerEXT = nullptr;
PFN_vkDestroyDebugUtilsMessengerEXT DestroyDebugMessengerEXT = nullptr;

void LoadVulkanDeviceFunctions(VkDevice device)
{
	// Get Descriptor Set Layout Size
	GetDescriptorSetLayoutSizeEXT = reinterpret_cast<PFN_vkGetDescriptorSetLayoutSizeEXT>(vkGetDeviceProcAddr(device, "vkGetDescriptorSetLayoutSizeEXT"));
	// Get Descriptor Set Layout Binding Offset
	GetDescriptorSetLayoutBindingOffsetEXT = reinterpret_cast<PFN_vkGetDescriptorSetLayoutBindingOffsetEXT>(vkGetDeviceProcAddr(device, "vkGetDescriptorSetLayoutBindingOffsetEXT"));
	// Get Descriptor EXT
	GetDescriptorEXT = reinterpret_cast<PFN_vkGetDescriptorEXT>(vkGetDeviceProcAddr(device, "vkGetDescriptorEXT"));
	// Bind Descriptor EXT
	BindDescriptorEXT = reinterpret_cast<PFN_vkCmdBindDescriptorBuffersEXT>(vkGetDeviceProcAddr(device, "vkCmdBindDescriptorBuffersEXT"));
	// Set Descriptor Buffer Offset EXT
	SetDescriptorBufferOffsetEXT = reinterpret_cast<PFN_vkCmdSetDescriptorBufferOffsetsEXT>(vkGetDeviceProcAddr(device, "vkCmdSetDescriptorBufferOffsetsEXT"));
}

void LoadVulkanInstanceFunctions(VkInstance instance)
{
	// Create Debug Messenger
	CreateDebugMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
	// Destroy Debug Messenger
	DestroyDebugMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
}
