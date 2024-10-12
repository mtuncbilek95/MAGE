#include <Engine/Core/Core.h>
#include <Engine/Window/WindowManager.h>

#include <Engine/VulkanGraphics/Instance/VulkanInstance.h>
#include <Engine/VulkanGraphics/Device/VulkanDevice.h>

#include <Engine/VulkanGraphics/Descriptor/VulkanDescLayout.h>
#include <Engine/VulkanGraphics/Descriptor/VulkanDescBuffer.h>

using namespace MAGE;

int main()
{
	SystemLog::Get().Initialize();
	IndWindowDesc windowProps = {
		.WindowRes = {1280, 720},
		.Mode = WindowMode::Windowed,
		.Title = "TestWindow"
	};
	Manager::Window::Get().InitWindow(windowProps);
	auto& window = Manager::Window::Get().GetWindow();

	InstanceProps instanceProps = {};
	instanceProps.appName = "TestApp";
	instanceProps.appVersion = Math::Vec3i(1, 0, 0);
	instanceProps.engineName = "MAGE";
	instanceProps.engineVersion = Math::Vec3i(1, 0, 0);

	VulkanInstance instance(instanceProps);

	DeviceProps deviceProps = {};
	deviceProps.m_computeQueueCount = 1;
	deviceProps.m_graphicsQueueCount = 1;
	deviceProps.m_transferQueueCount = 1;

	VulkanDevice device(deviceProps, &instance);

	DescLayoutProps layoutProps = {};
	layoutProps.bindings.push_back({ 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT });
	layoutProps.bindings.push_back({ 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT });
	layoutProps.bindings.push_back({ 2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT });
	layoutProps.initFlags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_DESCRIPTOR_BUFFER_BIT_EXT;
	VulkanDescLayout descLayout(layoutProps, &device);

	VulkanDescBuffer descBuffer(&descLayout, &device);

	spdlog::info("BufferSize: {}", descBuffer.GetTotalSize());
	spdlog::info("Offset: {}", descBuffer.GetOffset());

	window.Show();
	while (!window.IsClosed())
	{
		window.PollEvents();
	}

	return 0;
}