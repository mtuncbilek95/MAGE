#pragma once

#include "Engine/Core/Core.h"
#include "Editor/Object/IEditorObject.h"

namespace MAGE
{
	class EDITOR_API GuiWindow : public IEditorObject
	{
	public:
		virtual ~GuiWindow() = default;

		virtual void OnInitialize() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnPaint() = 0;
		virtual void OnClose() = 0;

		const String& GetWindowName() const { return mWindowName; }
		const Math::Vec2f& GetPosition() const { return mPosition; }
		const Math::Vec2f& GetWindowSize() const { return mWindowSize; }

		b8 IsOpen() const { return mIsOpen; }
		b8 IsDirty() const { return mIsDirty; }

		void SetOpen(b8 open) { mIsOpen = open; }
		void SetDirty(b8 dirty) { mIsDirty = dirty; }

	protected:
		String mWindowName;
		Math::Vec2f mPosition;
		Math::Vec2f mWindowSize;

		b8 mIsOpen;
		b8 mIsDirty;
	};
}
