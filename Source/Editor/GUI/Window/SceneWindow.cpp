#include "SceneWindow.h"

#include <imgui.h>

namespace MAGE
{
	SceneWindow::SceneWindow()
	{
	}

	SceneWindow::~SceneWindow()
	{
	}

	void SceneWindow::Update()
	{
		if (!m_dirty)
		{
			ImGui::SetWindowSize("Scenery", ImVec2(500, 500));
			m_dirty = true;
		}
	}

	void SceneWindow::Render()
	{
		ImGui::Begin("Scenery");
		// Get the region avail for render texture.
		// Send it to the secondary command buffer that will render the scene.
		// Render the scene to the render texture.
		// Put the VkImageView of the render texture to the ImGui::Image.
		ImGui::End();
	}
}