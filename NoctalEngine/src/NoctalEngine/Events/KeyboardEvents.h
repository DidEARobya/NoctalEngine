#pragma once
#include "Event.h"

namespace NoctalEngine
{
	class NOCTAL_ENGINE_API KeyEvent : public Event
	{
	public:
		inline KeyEvent(const uint32_t keyCode) : m_KeyCode{ keyCode } {}

		inline uint32_t GetKeyCode() const { return m_KeyCode; }
	protected:
		uint32_t m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		inline KeyPressedEvent(const uint32_t keyCode) : KeyEvent{ keyCode } {}

		EVENT_TYPE(KEY_PRESSED)
	};

	class KeyRepeatEvent : public KeyEvent
	{
	public:
		inline KeyRepeatEvent(const uint32_t keyCode, const uint32_t repeatCount) : KeyEvent{ keyCode }, m_RepeatCount(repeatCount) {}

		inline uint32_t GetRepeatCount() const { return m_RepeatCount; };

		EVENT_TYPE(KEY_REPEAT)

	private:
		uint32_t m_RepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		inline KeyReleasedEvent(const uint32_t keyCode) : KeyEvent{ keyCode } {}

		EVENT_TYPE(KEY_RELEASED)
	};
}