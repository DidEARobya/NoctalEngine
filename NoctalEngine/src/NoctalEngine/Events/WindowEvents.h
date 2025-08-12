#pragma once
#include "Event.h"

namespace NoctalEngine
{
    class WindowClosedEvent final : public Event
    {
    public:
        EVENT_TYPE(WINDOW_CLOSE)
    };
    class WindowResizeEvent final : public Event
    {
    public:
        WindowResizeEvent(const float width, const float height) : m_Width(width), m_Height(height) {}

        EVENT_TYPE(WINDOW_RESIZE)

        inline float GetWidth() const { return m_Width; }
        inline float GetHeight() const { return m_Height; }

    private:
        float m_Width;
        float m_Height;
    };
    class OnUpdateEvent final : public Event
    {
    public:
        EVENT_TYPE(ON_UPDATE)
    };
}