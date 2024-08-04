#pragma once

#include <Engine/Core/Core.h>
#include <Editor/GuiWindow/GuiWindow.h>

namespace MAGE
{
	class StyleWindow : public GuiWindow
	{
	public:
		StyleWindow();
		~StyleWindow() override;

		virtual void OnInitialize() override;
		virtual void OnUpdate() override;
		virtual void OnPaint() override;
		virtual void OnClose() override;

	private:
	};
}