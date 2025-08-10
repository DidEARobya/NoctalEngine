#pragma once
#include "nepch.h"
#include "NoctalEngine/Core.h"

namespace NoctalEngine
{
	enum class EventType
	{
		NONE = 0,

		ON_UPDATE,

		WINDOW_CLOSE,
		WINDOW_RESIZE,
		WINDOW_FOCUS,
		WINDOW_UNFOCUS,
		WINDOW_MOVE,

		KEY_PRESSED,
		KEY_REPEAT,
		KEY_RELEASED,

		MOUSE_BUTTON_PRESSED,
		MOUSE_BUTTON_RELEASED,
		MOUSE_MOVE,
		MOUSE_SCROLL
	};

#define NOCTAL_BIND_EVENT_FN(fn)[this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#define EVENT_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
                         virtual EventType GetEventType() const override { return GetStaticType(); }

	class NOCTAL_ENGINE_API Event
	{
		friend class EventDispatcher;

	public:
		Event() = default;
		virtual ~Event() = default;

		virtual EventType GetEventType() const = 0;
		inline bool IsHandled() const { return m_EventHandled; };

	protected:
		bool m_EventHandled = false;
	};

	class EventDispatcher
	{
	public:
		inline explicit EventDispatcher(Event& event) : m_Event(event) {}

		template<typename T>
		inline bool Dispatch(const std::function<bool(const T&)>& function)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_EventHandled = function(*static_cast<T*>(&m_Event));
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};
}