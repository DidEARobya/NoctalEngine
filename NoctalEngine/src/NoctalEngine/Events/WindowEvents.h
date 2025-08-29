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
        WindowResizeEvent(const uint32_t width, const uint32_t height) : m_Width(width), m_Height(height) 
        {
        }

        EVENT_TYPE(WINDOW_RESIZE)

        inline uint32_t GetWidth() const { return m_Width; }
        inline uint32_t GetHeight() const { return m_Height; }

    private:
        uint32_t m_Width;
        uint32_t m_Height;
    };
    class OnUpdateEvent final : public Event
    {
    public:
        EVENT_TYPE(ON_UPDATE)
    };
}