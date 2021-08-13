#pragma once

#include "Core/OS/KeyCodes.h"

#include <GLFW/glfw3.h>

namespace Razix
{
    namespace GLFWKeyCodes
    {
        static Razix::InputCode::Key GLFWToRazixKeyboardKey(uint32_t glfwKey)
        {
            static std::map<uint32_t, Razix::InputCode::Key> keyMap = {
                { GLFW_KEY_A, Razix::InputCode::Key::A },
                { GLFW_KEY_B, Razix::InputCode::Key::B },
                { GLFW_KEY_C, Razix::InputCode::Key::C },
                { GLFW_KEY_D, Razix::InputCode::Key::D },
                { GLFW_KEY_E, Razix::InputCode::Key::E },
                { GLFW_KEY_F, Razix::InputCode::Key::F },
                { GLFW_KEY_G, Razix::InputCode::Key::G },
                { GLFW_KEY_H, Razix::InputCode::Key::H },
                { GLFW_KEY_I, Razix::InputCode::Key::I },
                { GLFW_KEY_J, Razix::InputCode::Key::J },
                { GLFW_KEY_K, Razix::InputCode::Key::K },
                { GLFW_KEY_L, Razix::InputCode::Key::L },
                { GLFW_KEY_M, Razix::InputCode::Key::M },
                { GLFW_KEY_N, Razix::InputCode::Key::N },
                { GLFW_KEY_O, Razix::InputCode::Key::O },
                { GLFW_KEY_P, Razix::InputCode::Key::P },
                { GLFW_KEY_Q, Razix::InputCode::Key::Q },
                { GLFW_KEY_R, Razix::InputCode::Key::R },
                { GLFW_KEY_S, Razix::InputCode::Key::S },
                { GLFW_KEY_T, Razix::InputCode::Key::T },
                { GLFW_KEY_U, Razix::InputCode::Key::U },
                { GLFW_KEY_V, Razix::InputCode::Key::V },
                { GLFW_KEY_W, Razix::InputCode::Key::W },
                { GLFW_KEY_X, Razix::InputCode::Key::X },
                { GLFW_KEY_Y, Razix::InputCode::Key::Y },
                { GLFW_KEY_Z, Razix::InputCode::Key::Z },

                { GLFW_KEY_0, Razix::InputCode::Key::D0 },
                { GLFW_KEY_1, Razix::InputCode::Key::D1 },
                { GLFW_KEY_2, Razix::InputCode::Key::D2 },
                { GLFW_KEY_3, Razix::InputCode::Key::D3 },
                { GLFW_KEY_4, Razix::InputCode::Key::D4 },
                { GLFW_KEY_5, Razix::InputCode::Key::D5 },
                { GLFW_KEY_6, Razix::InputCode::Key::D6 },
                { GLFW_KEY_7, Razix::InputCode::Key::D7 },
                { GLFW_KEY_8, Razix::InputCode::Key::D8 },
                { GLFW_KEY_9, Razix::InputCode::Key::D9 },

                { GLFW_KEY_F1, Razix::InputCode::Key::F1 },
                { GLFW_KEY_F2, Razix::InputCode::Key::F2 },
                { GLFW_KEY_F3, Razix::InputCode::Key::F3 },
                { GLFW_KEY_F4, Razix::InputCode::Key::F4 },
                { GLFW_KEY_F5, Razix::InputCode::Key::F5 },
                { GLFW_KEY_F6, Razix::InputCode::Key::F6 },
                { GLFW_KEY_F7, Razix::InputCode::Key::F7 },
                { GLFW_KEY_F8, Razix::InputCode::Key::F8 },
                { GLFW_KEY_F9, Razix::InputCode::Key::F9 },
                { GLFW_KEY_F10, Razix::InputCode::Key::F10 },
                { GLFW_KEY_F11, Razix::InputCode::Key::F11 },
                { GLFW_KEY_F12, Razix::InputCode::Key::F12 },

                { GLFW_KEY_MINUS, Razix::InputCode::Key::Minus },
                { GLFW_KEY_DELETE, Razix::InputCode::Key::Delete },
                { GLFW_KEY_SPACE, Razix::InputCode::Key::Space },
                { GLFW_KEY_LEFT, Razix::InputCode::Key::Left },
                { GLFW_KEY_RIGHT, Razix::InputCode::Key::Right },
                { GLFW_KEY_UP, Razix::InputCode::Key::Up },
                { GLFW_KEY_DOWN, Razix::InputCode::Key::Down },
                { GLFW_KEY_LEFT_SHIFT, Razix::InputCode::Key::LeftShift },
                { GLFW_KEY_RIGHT_SHIFT, Razix::InputCode::Key::RightShift },
                { GLFW_KEY_ESCAPE, Razix::InputCode::Key::Escape },
                { GLFW_KEY_KP_ADD, Razix::InputCode::Key::A },
                { GLFW_KEY_COMMA, Razix::InputCode::Key::Comma },
                { GLFW_KEY_BACKSPACE, Razix::InputCode::Key::Backspace },
                { GLFW_KEY_ENTER, Razix::InputCode::Key::Enter },
                { GLFW_KEY_LEFT_SUPER, Razix::InputCode::Key::LeftSuper },
                { GLFW_KEY_RIGHT_SUPER, Razix::InputCode::Key::RightSuper },
                { GLFW_KEY_LEFT_ALT, Razix::InputCode::Key::LeftAlt },
                { GLFW_KEY_RIGHT_ALT, Razix::InputCode::Key::RightAlt },
                { GLFW_KEY_LEFT_CONTROL, Razix::InputCode::Key::LeftControl },
                { GLFW_KEY_RIGHT_CONTROL, Razix::InputCode::Key::RightControl }
            };

            return keyMap[glfwKey];
        }

        static Razix::InputCode::MouseKey GLFWToRazixMouseKey(uint32_t glfwKey)
        {

            static std::map<uint32_t, Razix::InputCode::MouseKey> keyMap = {
                { GLFW_MOUSE_BUTTON_LEFT, Razix::InputCode::MouseKey::ButtonLeft },
                { GLFW_MOUSE_BUTTON_RIGHT, Razix::InputCode::MouseKey::ButtonRight },
                { GLFW_MOUSE_BUTTON_MIDDLE, Razix::InputCode::MouseKey::ButtonMiddle }
            };

            return keyMap[glfwKey];
        }
    }
}
