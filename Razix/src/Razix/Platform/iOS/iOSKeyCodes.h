#pragma once

#include "Core/OS/KeyCodes.h"
#include <map>

namespace Razix
{
    namespace iOSKeyCodes
    {
        inline Razix::InputCode::Key iOSKeyToRazix(char key)
        {
            static std::map<char, Razix::InputCode::Key> keyMap = {
                { 'a', Razix::InputCode::Key::A },
                { 'b', Razix::InputCode::Key::B },
                { 'c', Razix::InputCode::Key::C },
                { 'd', Razix::InputCode::Key::D },
                { 'e', Razix::InputCode::Key::E },
                { 'f', Razix::InputCode::Key::F },
                { 'g', Razix::InputCode::Key::G },
                { 'h', Razix::InputCode::Key::H },
                { 'i', Razix::InputCode::Key::I },
                { 'j', Razix::InputCode::Key::J },
                { 'k', Razix::InputCode::Key::K },
                { 'l', Razix::InputCode::Key::L },
                { 'm', Razix::InputCode::Key::M },
                { 'n', Razix::InputCode::Key::N },
                { 'o', Razix::InputCode::Key::O },
                { 'p', Razix::InputCode::Key::P },
                { 'q', Razix::InputCode::Key::Q },
                { 'r', Razix::InputCode::Key::R },
                { 's', Razix::InputCode::Key::S },
                { 't', Razix::InputCode::Key::T },
                { 'u', Razix::InputCode::Key::U },
                { 'v', Razix::InputCode::Key::V },
                { 'w', Razix::InputCode::Key::W },
                { 'x', Razix::InputCode::Key::X },
                { 'y', Razix::InputCode::Key::Y },
                { 'z', Razix::InputCode::Key::Z },

                { '0', Razix::InputCode::Key::D0 },
                { '1', Razix::InputCode::Key::D1 },
                { '2', Razix::InputCode::Key::D2 },
                { '3', Razix::InputCode::Key::D3 },
                { '4', Razix::InputCode::Key::D4 },
                { '5', Razix::InputCode::Key::D5 },
                { '6', Razix::InputCode::Key::D6 },
                { '7', Razix::InputCode::Key::D7 },
                { '8', Razix::InputCode::Key::D8 },
                { '9', Razix::InputCode::Key::D9 },

                { '-', Razix::InputCode::Key::Minus },
                { 0x75, Razix::InputCode::Key::Delete },
                { ' ', Razix::InputCode::Key::Space },
                { 0x7B, Razix::InputCode::Key::Left },
                { 0x7C, Razix::InputCode::Key::Right },
                { 0x7E, Razix::InputCode::Key::Up },
                { 0x7D, Razix::InputCode::Key::Down },
                { 0x38, Razix::InputCode::Key::LeftShift },
                { 0x35, Razix::InputCode::Key::Escape },
                { '+', Razix::InputCode::Key::Equal },
                { 0x33, Razix::InputCode::Key::Backspace },
                { 0x24, Razix::InputCode::Key::Enter },
                { ',', Razix::InputCode::Key::Comma }
            };

            return keyMap[key];
        }

        inline Razix::InputCode::MouseKey iOSTouchToRazixMouseKey(uint32_t count)
        {
            if(count > 3)
                return Razix::InputCode::MouseKey::ButtonLeft;

            static std::map<uint32_t, Razix::InputCode::MouseKey> keyMap = {
                { 0, Razix::InputCode::MouseKey::ButtonLeft },
                { 1, Razix::InputCode::MouseKey::ButtonRight },
                { 2, Razix::InputCode::MouseKey::ButtonMiddle }
            };
            return keyMap[count];
        }
    }
}
