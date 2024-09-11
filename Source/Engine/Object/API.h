#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	template<typename TObject>
	class ENGINE_API API
	{
	public:
		virtual ~API() = default;

		static TObject* GetAPI()
		{
			if (!mInstance)
				CreateAPI();

			return mInstance.get();
		}

	protected:
		static void CreateAPI()
		{
			mInstance = MakeShared<TObject>();
		}

	private:
		static inline SharedPtr<TObject> mInstance;
	};
}
