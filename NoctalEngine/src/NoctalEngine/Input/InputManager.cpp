#include "nepch.h"
#include "InputManager.h"
#include "NoctalEngine/Events/MouseEvents.h"
#include "NoctalEngine/Events/KeyboardEvents.h"
#include "NoctalEngine/Window/Window.h"
#include "SDL3/SDL.h"

namespace NoctalEngine
{
    std::unordered_map<uint32_t, bool>    InputManager::m_KeyCodeMap = { };
    std::unordered_map<uint32_t, bool>    InputManager::m_MouseButtonMap = { };
    //ScarlettMath::Vec2                  InputManager::mMousePosition = { 0.0f, 0.0f };

    void InputManager::Update(Window* focussedWindow)
    {
        if (focussedWindow == nullptr)
        {
            NE_ENGINE_ERROR("InputManager attempted to update without a focussed Window (InputManager)");
            return;
        }

        SDL_Event input;

        while (SDL_PollEvent(&input))
        {
            if (input.type == SDL_EVENT_KEY_DOWN)
            {
                KeyPressedEvent event(input.key.scancode);
                focussedWindow->WindowData.eventCallback(event);
            }

            if (input.type == SDL_EVENT_KEY_UP)
            {
                KeyReleasedEvent event(input.key.scancode);
                focussedWindow->WindowData.eventCallback(event);
            }

            if (input.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                MouseButtonPressedEvent event(input.key.scancode);
                focussedWindow->WindowData.eventCallback(event);
            }

            if (input.type == SDL_EVENT_MOUSE_BUTTON_UP)
            {
                MouseButtonReleasedEvent event(input.key.scancode);
                focussedWindow->WindowData.eventCallback(event);
            }

            if (input.key.scancode == SDL_SCANCODE_ESCAPE)
            {
                //Game::Instance()->EndGame();
            }
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

        //dispatcher.Dispatch<MouseMovedEvent>([](const MouseMovedEvent& event)
        //    {
        //        mMousePosition = { event.GetXPos(), event.GetYPos() };
        //        return false;
        //    });
    }

}