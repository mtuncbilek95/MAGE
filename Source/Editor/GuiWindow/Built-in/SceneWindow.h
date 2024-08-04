#pragma once

#include <Engine/Core/Core.h>
#include <Editor/GuiWindow/GuiWindow.h>

namespace MAGE
{
	class SceneWindow : public GuiWindow
	{
	public:
		SceneWindow();
		~SceneWindow() override = default;

		virtual void OnInitialize() override;
		virtual void OnUpdate() override;
		virtual void OnPaint() override;
		virtual void OnClose() override;
	};
}
