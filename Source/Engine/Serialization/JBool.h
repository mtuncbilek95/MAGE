#pragma once

#include <Engine/Core/Core.h>
#include <Engine/Serialization/Core/JNode.h>

namespace MAGE
{
	class JBool : public JNode
	{
	public:
		JBool(const String& name, b8 value) : JNode(name), mValue(value) {}
		~JBool() = default;

		void SetValue(b8 value) { mValue = value; }

		FORCEINLINE b8 GetValue() const { return mValue; }
		NodeType GetType() const override { return NodeType::Boolean; }

	private:
		b8 mValue;
	};
}
