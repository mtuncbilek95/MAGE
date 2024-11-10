#include "VInstance.h"

#include "Engine/Vulkan/Core/VkAssert.h"

namespace MAGE
{
	static VkBool32 DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		String prefix;
		if (messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
			prefix = "[Performance]";
		else if (messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT)
			prefix = "[Address Binding]";
		else if (messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
			prefix = "[Validation]";
		else if (messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT)
			prefix = "[General]";

		switch (messageSeverity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
			spdlog::debug("{0}-{1}", std::string_view(prefix), pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
			spdlog::trace("{0}-{1}", std::string_view(prefix), pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
			spdlog::warn("{0}-{1}", std::string_view(prefix), pCallbackData->pMessage);
			break;
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
			spdlog::error("{0}-{1}", std::string_view(prefix), pCallbackData->pMessage);
			break;
		default:
			break;
		}

		return false;
	}

	VInstance::VInstance(const InstanceProps& desc) : m_props(desc)
	{
		struct ExtensionEntry
		{
			const char* m_name;
			bool m_support;
		};

		Vector<ExtensionEntry> extensions;
		Vector<const char*> workingExtensions;
		extensions.push_back({ VK_KHR_SURFACE_EXTENSION_NAME, false });
		extensions.push_back({ VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME, false });

#if defined(DELUSION_WINDOWS)
		extensions.push_back({ VK_KHR_WIN32_SURFACE_EXTENSION_NAME, false });
#endif // DELUSION_WINDOWS

#if defined(DELUSION_DEBUG)
		extensions.push_back({ VK_EXT_DEBUG_UTILS_EXTENSION_NAME, false });
		extensions.push_back({ VK_EXT_DEBUG_REPORT_EXTENSION_NAME, false });
#endif // DELUSION_DEBUG

		u32 extensionCount = 0;
		ErrorUtils::VkAssert(vk::enumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr), "VInstance");

		Vector<vk::ExtensionProperties> allExtensions(extensionCount);
		ErrorUtils::VkAssert(vk::enumerateInstanceExtensionProperties(nullptr, &extensionCount, allExtensions.data()), "VInstance");

		for (usize i = 0; i < extensions.size(); ++i) {
			for (auto& extension : allExtensions) {
				if (strcmp(extensions[i].m_name, extension.extensionName) == 0) {
					extensions[i].m_support = true;
					workingExtensions.push_back(extensions[i].m_name);
					break;
				}
			}
		}

		u32 layerCount = 0;
		ErrorUtils::VkAssert(vk::enumerateInstanceLayerProperties(&layerCount, nullptr), "VInstance");
		Vector<vk::LayerProperties> allLayers(layerCount);
		ErrorUtils::VkAssert(vk::enumerateInstanceLayerProperties(&layerCount, allLayers.data()), "VInstance");

		Vector<ExtensionEntry> wantedLayers;
		Vector<const char*> workingLayers;
#if defined(DELUSION_DEBUG)

#if defined(RENDER_DOC)
		wantedLayers.push_back({ "VK_LAYER_RENDERDOC_Capture", false });
#endif
		wantedLayers.push_back({ "VK_LAYER_KHRONOS_synchronization2", false });
		wantedLayers.push_back({ "VK_LAYER_KHRONOS_validation", false });
#endif

		for (usize i = 0; i < wantedLayers.size(); ++i)
		{
			for (auto& layer : allLayers)
			{
				if (strcmp(wantedLayers[i].m_name, layer.layerName) == 0)
				{
					wantedLayers[i].m_support = true;
					workingLayers.push_back(wantedLayers[i].m_name);
					break;
				}
			}
		}

		// Print unsupported layers
		for (auto& layer : wantedLayers)
		{
			if (!layer.m_support)
				spdlog::warn("Layer not supported: {}", layer.m_name);
		}

		// Print unsupported extensions
		for (auto& extension : extensions)
		{
			if (!extension.m_support)
				spdlog::warn("Extension not supported: {}", extension.m_name);
		}

		vk::ApplicationInfo appInfo(
			desc.appName.Data(),
			VK_MAKE_VERSION(desc.appVersion.x, desc.appVersion.y, desc.appVersion.z),
			desc.engineName.Data(),
			VK_MAKE_VERSION(desc.engineVersion.x, desc.engineVersion.y, desc.engineVersion.z),
			VK_API_VERSION_1_3
		);

		vk::InstanceCreateInfo instanceInfo(
			{}, &appInfo,
			workingLayers.size(), workingLayers.data(),
			workingExtensions.size(), workingExtensions.data());

		ErrorUtils::VkAssert(vk::createInstance(&instanceInfo, nullptr, &m_instance), "VInstance");
		m_loader.init(m_instance, vkGetInstanceProcAddr);

#if defined(DELUSION_DEBUG)
		vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo(
			{},
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo |
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
			vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
			vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eDeviceAddressBinding,
			DebugCallback);

		m_debugger = m_instance.createDebugUtilsMessengerEXT(debugCreateInfo, nullptr, m_loader);
#endif // DELUSION_DEBUG

		u32 deviceCount = 0;
		ErrorUtils::VkAssert(m_instance.enumeratePhysicalDevices(&deviceCount, nullptr), "VInstance");

		// Temporary struct to hold the device and its score
		Map<String, Pair<vk::PhysicalDevice, u32>> allDevices;

		// Get the physical devices
		Vector<vk::PhysicalDevice> devices(deviceCount);
		ErrorUtils::VkAssert(m_instance.enumeratePhysicalDevices(&deviceCount, devices.data()), "VInstance");

		for (auto& device : devices)
		{
			// Get the device properties
			vk::PhysicalDeviceProperties deviceProperties = {};
			device.getProperties(&deviceProperties);

			// Get the device features
			vk::PhysicalDeviceFeatures deviceFeatures = {};
			device.getFeatures(&deviceFeatures);

			// Get the device memory properties
			vk::PhysicalDeviceMemoryProperties deviceMemoryProperties = {};
			device.getMemoryProperties(&deviceMemoryProperties);

			// Get the device queue family properties
			u32 queueFamilyCount = 0;
			device.getQueueFamilyProperties(&queueFamilyCount, nullptr);

			Vector<vk::QueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
			device.getQueueFamilyProperties(&queueFamilyCount, queueFamilyProperties.data());

			// Hold device on allDevices
			allDevices[deviceProperties.deviceName.data()] = { device, 0 };

			for (auto& queueFamily : queueFamilyProperties)
			{
				allDevices[deviceProperties.deviceName.data()].second += queueFamily.queueCount;
			}

			// Check if the device is discrete
			if (deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
				allDevices[deviceProperties.deviceName.data()].second += 1000;
			else
				allDevices[deviceProperties.deviceName.data()].second += 100;

			// Check if the device supports geometry shaders
			if (deviceFeatures.geometryShader)
				allDevices[deviceProperties.deviceName.data()].second += 10;
		}

		// Get the best device
		auto bestDevice = std::max_element(allDevices.begin(), allDevices.end(),
			[](const Pair<String, Pair<vk::PhysicalDevice, u32>>& a, const Pair<String, Pair<vk::PhysicalDevice, u32>>& b)
			{
				return a.second.second < b.second.second;
			});

		spdlog::info("Best device found: {}", bestDevice->first.CharString());
		m_adapter = bestDevice->second.first;
	}

	VInstance::~VInstance()
	{
		Destroy();
	}

	void VInstance::Destroy()
	{
#if defined(DELUSION_DEBUG)
		if (m_debugger != VK_NULL_HANDLE)
		{
			m_instance.destroyDebugUtilsMessengerEXT(m_debugger, nullptr, m_loader);
			m_debugger = VK_NULL_HANDLE;
		}
#endif
		if (m_instance != VK_NULL_HANDLE)
		{
			m_instance.destroy();
			m_instance = VK_NULL_HANDLE;
		}
	}
}
