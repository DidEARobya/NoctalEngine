#pragma once
#include <glm/glm.hpp>

namespace NoctalEngine
{
    class Event;
    class Window;

    class InputManager
    {
        friend class Engine;
    public:
        static void Update(Window* focussedWindow);

        static inline bool IsKeyDown(const uint32_t keyCode) { return m_KeyCodeMap[keyCode]; }
        static inline bool IsMouseButtonDown(const uint8_t mouseCode) { return m_MouseButtonMap[mouseCode]; }
        static void OnEvent(Event& e);

        static inline const glm::vec2& GetMousePosition() { return m_MousePosition; };

        //[[nodiscard]] static inline ScarlettMath::Vec2 GetMousePosition() { return mMousePosition; }
    private:
        static std::unordered_map<uint32_t, bool> m_KeyCodeMap;
        static std::unordered_map<uint8_t, bool> m_MouseButtonMap;

        static glm::vec2 m_MousePosition;
    };
}