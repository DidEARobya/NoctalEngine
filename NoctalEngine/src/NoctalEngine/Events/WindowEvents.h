#pragma once
#include "Event.h"

namespace NoctalEngine
{
    class WindowClosedEvent final : public Event
    {
    public:
        EVENT_TYPE(WINDOW_CLOSE)
    };

    class OnUpdateEvent final : public Event
    {
    public:
        EVENT_TYPE(ON_UPDATE)
    };
}