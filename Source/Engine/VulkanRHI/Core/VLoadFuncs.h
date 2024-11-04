/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2024 Metehan Tuncbilek
 */

#pragma once

#include "Engine/Core/Core.h"
#include <vulkan/vulkan.h>

// Define the function pointers for the Vulkan functions

extern PFN_vkGetDescriptorSetLayoutSizeEXT GetDescriptorSetLayoutSizeEXT;
extern PFN_vkGetDescriptorSetLayoutBindingOffsetEXT GetDescriptorSetLayoutBindingOffsetEXT;

extern PFN_vkCreateDebugUtilsMessengerEXT CreateDebugMessengerEXT;
extern PFN_vkDestroyDebugUtilsMessengerEXT DestroyDebugMessengerEXT;

extern PFN_vkGetDescriptorEXT GetDescriptorEXT;
extern PFN_vkCmdBindDescriptorBuffersEXT BindDescriptorEXT;
extern PFN_vkCmdSetDescriptorBufferOffsetsEXT SetDescriptorBufferOffsetEXT;

void LoadVulkanDeviceFunctions(VkDevice device);
void LoadVulkanInstanceFunctions(VkInstance instance);

