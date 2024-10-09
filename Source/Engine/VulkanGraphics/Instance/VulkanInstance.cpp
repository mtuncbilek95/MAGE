#include "VulkanInstance.h"

#include "Engine/ErrorHandler/ErrorChecker.h"

namespace MAGE
{
	VulkanInstance::VulkanInstance(const InstanceProps& desc)
	{
		struct ExtensionEntry
		{
			const char* m_name;
			bool m_instanceSupport;
		};

		// Fill the wanted extensions
		std::vector<ExtensionEntry> extensions;
		extensions.push_back({ VK_KHR_SURFACE_EXTENSION_NAME, false });

#if defined(DELUSION_WINDOWS)
		extensions.push_back({ VK_KHR_WIN32_SURFACE_EXTENSION_NAME, false });
#endif

#if defined(DELUSION_DEBUG)
		extensions.push_back({ VK_EXT_DEBUG_UTILS_EXTENSION_NAME, false });
		extensions.push_back({ VK_EXT_DEBUG_REPORT_EXTENSION_NAME, false });
#endif




		vk::ApplicationInfo appInfo(
			desc.AppName.c_str(),
			VK_MAKE_VERSION(desc.AppVersion.x, desc.AppVersion.y, desc.AppVersion.z),
			desc.EngineName.c_str(),
			VK_MAKE_VERSION(desc.EngineVersion.x, desc.EngineVersion.y, desc.EngineVersion.z)
		);

		vk::InstanceCreateInfo createInfo(
			vk::InstanceCreateFlags(),
			&appInfo
		);

	}
}
