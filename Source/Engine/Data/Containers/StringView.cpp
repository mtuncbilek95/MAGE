#include "StringView.h"
#include "String.h"

namespace MAGE
{
	StringView::StringView(const String& str) : m_view(str)
	{
	}
}
