#pragma once

#include "Engine/Core/Core.h"

namespace MAGE
{
	/**
	* @class ENGINE_API Event
	* @brief Base class ENGINE_API for the event types. If you need to create a new event type, 
	* you should inherit from this class ENGINE_API.
	*/
	template<typename Event0>
	class ENGINE_API Event
	{
		typedef Event0 MainEvent;
	public:
		Event() = default;
		Event(MainEvent type, const String& name = "") : mEventType(type), mEventName(name) {}
		~Event() = default;

		MainEvent GetEventType() const { return mEventType; }
		const String& GetEventName() const { return mEventName; }

		template<typename Event1>
		Event1 ToType() { return static_cast<Event1>(MainEvent); }

	private:
		MainEvent mEventType;
		String mEventName;
	};
}
