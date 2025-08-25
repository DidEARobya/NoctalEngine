#include "nepch.h"
#include "InputManager.h"
#include "NoctalEngine/Events/MouseEvents.h"
#include "NoctalEngine/Events/KeyboardEvents.h"
#include "NoctalEngine/Events/WindowEvents.h"
#include "NoctalEngine/Window/Window.h"
#include "KeyCodes.h"
#include "SDL3/SDL.h"
#include "backends/imgui_impl_sdl3.h"

namespace NoctalEngine
{
    std::unordered_map<uint32_t, bool>      InputManager::m_KeyCodeMap = { };
    std::unordered_map<uint8_t, bool>       InputManager::m_MouseButtonMap = { };
    glm::vec2                               InputManager::m_MousePosition = { 0.0f, 0.0f };

    void InputManager::Update(Window* focussedWindow)
    {
        if (focussedWindow == nullptr)
        {
            NE_ENGINE_ERROR("InputManager attempted to update without a focussed Window");
            return;
        }

        SDL_Event input;

        while (SDL_PollEvent(&input))
        {
            ImGui_ImplSDL3_ProcessEvent(&input);

            //---- WINDOW EVENTS ----
            if (input.type == SDL_EventType::SDL_EVENT_WINDOW_CLOSE_REQUESTED)
            {
                WindowClosedEvent event;
                focussedWindow->WindowData.eventCallback(event);
            }
            if (input.type == SDL_EventType::SDL_EVENT_WINDOW_RESIZED)
            {
                WindowResizeEvent event((float)focussedWindow->GetWidth(), (float)focussedWindow->GetHeight());
                focussedWindow->WindowData.eventCallback(event);
            }
            //---- WINDOW EVENTS ----

            //---- KEY EVENTS ----
            if (input.type == SDL_EventType::SDL_EVENT_KEY_DOWN)
            {
                KeyPressedEvent event(input.key.key, input.key.scancode);
                focussedWindow->WindowData.eventCallback(event);
            }

            if (input.type == SDL_EventType::SDL_EVENT_KEY_UP)
            {
                KeyReleasedEvent event(input.key.key, input.key.scancode);
                focussedWindow->WindowData.eventCallback(event);
            }
            
            if (input.type == SDL_EventType::SDL_EVENT_TEXT_INPUT)
            {
                KeyTypedEvent event(input.key.key, input.key.scancode);
                focussedWindow->WindowData.eventCallback(event);
            }
            //---- KEY EVENTS ----
            
            //---- MOUSE EVENTS ----
            if (input.type == SDL_EventType::SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                MouseButtonPressedEvent event(input.button.button);
                focussedWindow->WindowData.eventCallback(event);
            }

            if (input.type == SDL_EventType::SDL_EVENT_MOUSE_BUTTON_UP)
            {
                MouseButtonReleasedEvent event(input.button.button);
                focussedWindow->WindowData.eventCallback(event);
            }

            if (input.type == SDL_EventType::SDL_EVENT_MOUSE_MOTION)
            {
                float x;
                float y;

                SDL_GetMouseState(&x, &y);

                m_MousePosition = glm::vec2(x, y);

                MouseMovedEvent event(x, y);
                focussedWindow->WindowData.eventCallback(event);
            }
            if (input.type == SDL_EventType::SDL_EVENT_MOUSE_WHEEL)
            {
                MouseScrollEvent event((float)input.wheel.integer_x, (float)input.wheel.integer_y);
                focussedWindow->WindowData.eventCallback(event);
            }
            //---- MOUSE EVENTS ----
        }
    }

    void InputManager::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);

        dispatcher.Dispatch<KeyPressedEvent>([](const KeyPressedEvent& event)
            {
                m_KeyCodeMap[event.GetKeyCode()] = true;
                return false;
            });

        dispatcher.Dispatch<KeyReleasedEvent>([](const KeyReleasedEvent& event)
            {
                m_KeyCodeMap[event.GetKeyCode()] = false;
                return false;
            });

        dispatcher.Dispatch<MouseButtonPressedEvent>([](const MouseButtonPressedEvent& event)
            {
                m_MouseButtonMap[event.GetMouseButton()] = true;
                return false;
            });

        dispatcher.Dispatch<MouseButtonReleasedEvent>([](const MouseButtonReleasedEvent& event)
            {
                m_MouseButtonMap[event.GetMouseButton()] = false;
                return false;
            });
    }
}