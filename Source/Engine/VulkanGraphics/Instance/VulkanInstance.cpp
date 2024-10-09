#include "VulkanInstance.h"

#include "Engine/VulkanGraphics/Core/VkAssert.h"

#if defined(DELUSION_WINDOWS)
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#endif

namespace MAGE
{
#if defined(DELUSION_DEBUG)
	PFN_vkCreateDebugUtilsMessengerEXT debugMessengerCreateProc = nullptr;
	PFN_vkDestroyDebugUtilsMessengerEXT debugMessengerDestroyProc = nullptr;

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		switch (messageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			spdlog::trace("{}", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			spdlog::info("{}", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			spdlog::warn("{}", pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			spdlog::error("{}", pCallbackData->pMessage);
			break;
		default:
			break;
		}

		return VK_FALSE;
	}
#endif // DELUSION_DEBUG

	VulkanInstance::VulkanInstance(const InstanceProps& desc) : m_props(desc)
		, m_instance(VK_NULL_HANDLE)
		, m_adapter(VK_NULL_HANDLE)
	{
#if defined(DELUSION_DEBUG)
		m_debugMessenger = VK_NULL_HANDLE;
#endif
		 
		struct ExtensionEntry
		{
			const char* m_name;
			bool m_support;
		};

		Vector<ExtensionEntry> extensions;
		extensions.push_back({ VK_KHR_SURFACE_EXTENSION_NAME, false });

#if defined(DELUSION_WINDOWS)
		extensions.push_back({ VK_KHR_WIN32_SURFACE_EXTENSION_NAME, false });
#endif // DELUSION_WINDOWS

#if defined(DELUSION_DEBUG)
		extensions.push_back({ VK_EXT_DEBUG_UTILS_EXTENSION_NAME, false });
		extensions.push_back({ VK_EXT_DEBUG_REPORT_EXTENSION_NAME, false });
#endif // DELUSION_DEBUG

		u32 extensionCount = 0;
		ErrorUtils::VkAssert(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr), "VulkanInstance");

		Vector<VkExtensionProperties> allExtensions(extensionCount);
		ErrorUtils::VkAssert(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, allExtensions.data()), "VulkanInstance");

		for (usize i = 0; i < extensions.size(); ++i) {
			for (auto& extension : allExtensions) {
				if (strcmp(extensions[i].m_name, extension.extensionName) == 0) {
					extensions[i].m_support = true;
					break;
				}
			}
		}

		Vector<const char*> workingExtensions;
		Vector<const char*> brokenExtensions;

		for (auto& extension : extensions) {
			if (extension.m_support)
				workingExtensions.push_back(extension.m_name);
			else
				brokenExtensions.push_back(extension.m_name);
		}

		u32 layerCount = 0;
		ErrorUtils::VkAssert(vkEnumerateInstanceLayerProperties(&layerCount, nullptr), "VulkanInstance");

		Vector<VkLayerProperties> allLayers(layerCount);
		ErrorUtils::VkAssert(vkEnumerateInstanceLayerProperties(&layerCount, allLayers.data()), "VulkanInstance");

		Vector<ExtensionEntry> wantedLayers;

#if defined(DELUSION_DEBUG)
		wantedLayers.push_back({ "VK_LAYER_KHRONOS_validation", false });
		wantedLayers.push_back({ "VK_LAYER_LUNARG_screenshot" , false });
#endif

		for (usize i = 0; i < wantedLayers.size(); ++i)
		{
			for (auto& layer : allLayers)
			{
				if (strcmp(wantedLayers[i].m_name, layer.layerName) == 0)
				{
					wantedLayers[i].m_support = true;
					break;
				}
			}
		}

		Vector<const char*> workingLayers;
		Vector<const char*> brokenLayers;

		for (auto& layer : wantedLayers)
		{
			if (layer.m_support)
				workingLayers.push_back(layer.m_name);
			else
				brokenLayers.push_back(layer.m_name);
		}

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = desc.AppName.c_str();
		appInfo.applicationVersion = VK_MAKE_VERSION(desc.AppVersion.x, desc.AppVersion.y, desc.AppVersion.z);
		appInfo.pEngineName = desc.EngineName.c_str();
		appInfo.engineVersion = VK_MAKE_VERSION(desc.EngineVersion.x, desc.EngineVersion.y, desc.EngineVersion.z);
		appInfo.apiVersion = VK_API_VERSION_1_3;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = static_cast<u32>(workingExtensions.size());
		createInfo.ppEnabledExtensionNames = workingExtensions.data();
		createInfo.enabledLayerCount = static_cast<u32>(workingLayers.size());
		createInfo.ppEnabledLayerNames = workingLayers.data();

		ErrorUtils::VkAssert(vkCreateInstance(&createInfo, nullptr, &m_instance), "VulkanInstance");

#if defined(DELUSION_DEBUG)
		debugMessengerCreateProc = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT"));
		debugMessengerDestroyProc = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT"));

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
		debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugCreateInfo.pfnUserCallback = DebugCallback;
		debugCreateInfo.pUserData = nullptr;

		ErrorUtils::VkAssert(debugMessengerCreateProc(m_instance, &debugCreateInfo, nullptr, &m_debugMessenger), "VulkanInstance");
#endif // DELUSION_DEBUG

		u32 deviceCount = 0;
		ErrorUtils::VkAssert(vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr), "VulkanInstance");

		// Temporary struct to hold the device and its score
		Map<String, Pair<VkPhysicalDevice, u32>> allDevices;

		// Get the physical devices
		Vector<VkPhysicalDevice> devices(deviceCount);
		ErrorUtils::VkAssert(vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data()), "VInsVulkanInstancetance");

		for (auto& device : devices)
		{
			// Get the device properties
			VkPhysicalDeviceProperties deviceProperties = {};
			vkGetPhysicalDeviceProperties(device, &deviceProperties);

			// Get the device features
			VkPhysicalDeviceFeatures deviceFeatures = {};
			vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

			// Get the device memory properties
			VkPhysicalDeviceMemoryProperties deviceMemoryProperties = {};
			vkGetPhysicalDeviceMemoryProperties(device, &deviceMemoryProperties);

			// Get the device queue family properties
			u32 queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

			Vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyProperties.data());

			// Hold device on allDevices
			allDevices[deviceProperties.deviceName] = { device, 0 };

			for (auto& queueFamily : queueFamilyProperties)
			{
				allDevices[deviceProperties.deviceName].second += queueFamily.queueCount;
			}

			// Check if the device is discrete
			if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
				allDevices[deviceProperties.deviceName].second += 1000;
			else
				allDevices[deviceProperties.deviceName].second += 100;

			// Check if the device supports geometry shaders
			if (deviceFeatures.geometryShader)
				allDevices[deviceProperties.deviceName].second += 10;
		}

		// Get the best device
		auto bestDevice = std::max_element(allDevices.begin(), allDevices.end(),
			[](const Pair<String, Pair<VkPhysicalDevice, u32>>& a, const Pair<String, Pair<VkPhysicalDevice, u32>>& b)
			{
				return a.second.second < b.second.second;
			});

		spdlog::trace("Best device found: {}", bestDevice->first.c_str());
		m_adapter = bestDevice->second.first;
	}

	VulkanInstance::~VulkanInstance()
	{
#if defined(DELUSION_DEBUG)
		debugMessengerDestroyProc(m_instance, m_debugMessenger, nullptr);
#endif // DELUSION_DEBUG

		vkDestroyInstance(m_instance, nullptr);
	}
}
