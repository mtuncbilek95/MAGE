#pragma once

#include "Engine/Core/Core.h"
#include "Editor/GuiWindow/GuiWindow.h"

namespace MAGE
{
	class MenuBar;

	class EDITOR_API DockWindow : public GuiWindow
	{
	public:
		DockWindow();
		~DockWindow() override = default;

		virtual void OnInitialize() override;
		virtual void OnUpdate() override;
		virtual void OnPaint() override;
		virtual void OnClose() override;

	private:
		SharedPtr<MenuBar> mMenuBar;
	};
}
