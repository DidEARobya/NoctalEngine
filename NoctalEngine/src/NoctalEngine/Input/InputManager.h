#pragma once
namespace NoctalEngine
{
    class Event;
    class Window;

    class InputManager
    {
        friend class Engine;
    public:
        static void Update(Window* focussedWindow);

        static inline bool IsKeyDown(const uint16_t keyCode) { return m_KeyCodeMap[keyCode]; }
        static inline bool IsMouseButtonDown(const uint16_t mouseCode) { return m_MouseButtonMap[mouseCode]; }
        static void OnEvent(Event& e);

        static void GetMousePosition(float& mouseX, float& mouseY);
        //[[nodiscard]] static inline ScarlettMath::Vec2 GetMousePosition() { return mMousePosition; }
    private:
        static std::unordered_map<uint32_t, bool> m_KeyCodeMap;
        static std::unordered_map<uint32_t, bool> m_MouseButtonMap;
    };
}