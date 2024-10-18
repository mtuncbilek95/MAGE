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
		ImGui::Text("Hello, World!");
		ImGui::End();
	}
}