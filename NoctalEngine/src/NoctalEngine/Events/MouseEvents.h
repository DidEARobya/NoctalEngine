#pragma once
#include "Event.h"

namespace NoctalEngine
{
    class MouseEvent : public Event
    {
    public:
        inline explicit MouseEvent(const float mouseX, const float mouseY) : m_MouseX(mouseX), m_MouseY(mouseY) {}

        inline float GetMouseXPos() const { return m_MouseX; }
        inline float GetMouseYPos() const { return m_MouseY; }
    protected:
        float m_MouseX = 0.0f;
        float m_MouseY = 0.0f;
    };

    class MouseMoveEvent final : public MouseEvent
    {
    public:
        inline MouseMoveEvent(const float mouseX, const float mouseY) : MouseEvent(mouseX, mouseY) {}

        EVENT_TYPE(MOUSE_MOVE)
    };

    class MouseScrollEvent final : public Event
    {
    public:
        inline MouseScrollEvent(const float mouseX, const float mouseY) : m_ScrollXOffset(mouseX), m_ScrollYOffset(mouseY) {}

        EVENT_TYPE(MOUSE_SCROLL)

        inline float GetXOffset() const { return m_ScrollXOffset; }
        inline float GetYOffset() const { return m_ScrollYOffset; }
    private:
        float m_ScrollXOffset = 0.0f;
        float m_ScrollYOffset = 0.0f;
    };

    class MouseButtonEvent : public Event
    {
    public:
        inline explicit MouseButtonEvent(const uint32_t button) : m_Button(button) {}

        inline uint32_t GetMouseButton() const { return m_Button; }
    protected:
        uint32_t m_Button;
    };

    class MouseButtonPressedEvent final : public MouseButtonEvent
    {
    public:
        MouseButtonPressedEvent(const uint32_t button) : MouseButtonEvent{ button } {}

        EVENT_TYPE(MOUSE_BUTTON_PRESSED)
    };

    class MouseButtonReleasedEvent final : public MouseButtonEvent
    {
    public:
        MouseButtonReleasedEvent(const uint32_t button) : MouseButtonEvent{ button } {}

        EVENT_TYPE(MOUSE_BUTTON_RELEASED)
    };
}