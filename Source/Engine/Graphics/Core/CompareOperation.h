#pragma once

namespace MAGE
{
		enum class CompareOperation : u32
		{
			Never,
			Less,
			Equal,
			LessEqual,
			Greater,
			NotEqual,
			GreaterEqual,
			Always
		};

	GENERATE_ENUM_FLAG(MAGE::CompareOperation, u32);
}