#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Object/API.h"

#include "Editor/ImguiRenderer/ImGuiRenderer.h"

namespace MAGE
{
	class EDITOR_API ImGuiAPI : public API<ImGuiAPI>
	{
		friend class EDITOR_API ImGuiRenderModule;

	public:
		ImGuiAPI() = default;
		virtual ~ImGuiAPI() override = default;

		ImGuiRenderer* GetEditorRenderer() { return mEditorRenderer.get(); }

	private:
		ImGuiRenderer* CreateEditor()
		{
			if (!mEditorRenderer)
				mEditorRenderer = MakeShared<ImGuiRenderer>();

			return mEditorRenderer.get();
		}

	private:
		SharedPtr<ImGuiRenderer> mEditorRenderer;
	};
}
