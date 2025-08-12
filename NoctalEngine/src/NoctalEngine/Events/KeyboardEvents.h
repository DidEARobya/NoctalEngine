#pragma once
#include "Event.h"

namespace NoctalEngine
{
	class NOCTAL_ENGINE_API KeyEvent : public Event
	{
	public:
		inline KeyEvent(const uint32_t keyCode, const uint32_t scanCode) : m_KeyCode{ keyCode }, m_ScanCode{ scanCode } {}

		inline uint32_t GetKeyCode() const { return m_KeyCode; }
		inline uint32_t GetScanCode() const { return m_ScanCode; }

	protected:
		uint32_t m_KeyCode;
		uint32_t m_ScanCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		inline KeyPressedEvent(const uint32_t keyCode, const uint32_t scanCode) : KeyEvent{ keyCode, scanCode } {}

		EVENT_TYPE(KEY_PRESSED)
	};

	class KeyRepeatEvent : public KeyEvent
	{
	public:
		inline KeyRepeatEvent(const uint32_t keyCode, const uint32_t scanCode, const uint32_t repeatCount) : KeyEvent{ keyCode, scanCode }, m_RepeatCount(repeatCount) {}

		inline uint32_t GetRepeatCount() const { return m_RepeatCount; };

		EVENT_TYPE(KEY_REPEAT)

	private:
		uint32_t m_RepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		inline KeyReleasedEvent(const uint32_t keyCode, const uint32_t scanCode) : KeyEvent{ keyCode, scanCode } {}

		EVENT_TYPE(KEY_RELEASED)
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		inline KeyTypedEvent(const uint32_t keyCode, const uint32_t scanCode) : KeyEvent{ keyCode, scanCode } {}

		EVENT_TYPE(KEY_TYPED)
	};
}