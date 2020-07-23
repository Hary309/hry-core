#pragma once

#include <array>
#include <string>
#include <variant>

#include "Hry/System/Keyboard.hpp"
#include "Hry/System/Mouse.hpp"

namespace hry
{
struct BindableKey
{
    using Key_t = std::variant<Keyboard::Key, Mouse::Button>; // TODO: Add controller's keys

    Key_t key;
    std::string name;

    template<typename T>
    bool has() const
    {
        return std::holds_alternative<T>(key);
    }
};

static inline const auto BindableKeys = std::array{
    // Keyboard
    BindableKey{ Keyboard::Key::A, "A" },
    BindableKey{ Keyboard::Key::B, "B" },
    BindableKey{ Keyboard::Key::C, "C" },
    BindableKey{ Keyboard::Key::D, "D" },
    BindableKey{ Keyboard::Key::E, "E" },
    BindableKey{ Keyboard::Key::F, "F" },
    BindableKey{ Keyboard::Key::G, "G" },
    BindableKey{ Keyboard::Key::H, "H" },
    BindableKey{ Keyboard::Key::I, "I" },
    BindableKey{ Keyboard::Key::J, "J" },
    BindableKey{ Keyboard::Key::K, "K" },
    BindableKey{ Keyboard::Key::L, "L" },
    BindableKey{ Keyboard::Key::M, "M" },
    BindableKey{ Keyboard::Key::N, "N" },
    BindableKey{ Keyboard::Key::O, "O" },
    BindableKey{ Keyboard::Key::P, "P" },
    BindableKey{ Keyboard::Key::Q, "Q" },
    BindableKey{ Keyboard::Key::R, "R" },
    BindableKey{ Keyboard::Key::S, "S" },
    BindableKey{ Keyboard::Key::T, "T" },
    BindableKey{ Keyboard::Key::U, "U" },
    BindableKey{ Keyboard::Key::V, "V" },
    BindableKey{ Keyboard::Key::W, "W" },
    BindableKey{ Keyboard::Key::X, "X" },
    BindableKey{ Keyboard::Key::Y, "Y" },
    BindableKey{ Keyboard::Key::Z, "Z" },
    BindableKey{ Keyboard::Key::Num0, "0" },
    BindableKey{ Keyboard::Key::Num1, "1" },
    BindableKey{ Keyboard::Key::Num2, "2" },
    BindableKey{ Keyboard::Key::Num3, "3" },
    BindableKey{ Keyboard::Key::Num4, "4" },
    BindableKey{ Keyboard::Key::Num5, "5" },
    BindableKey{ Keyboard::Key::Num6, "6" },
    BindableKey{ Keyboard::Key::Num7, "7" },
    BindableKey{ Keyboard::Key::Num8, "8" },
    BindableKey{ Keyboard::Key::Num9, "9" },
    BindableKey{ Keyboard::Key::Escape, "Escape" },
    BindableKey{ Keyboard::Key::LControl, "Left Control" },
    BindableKey{ Keyboard::Key::LShift, "Left Shift" },
    BindableKey{ Keyboard::Key::LAlt, "Left Alt" },
    BindableKey{ Keyboard::Key::LSystem, "Left System" },
    BindableKey{ Keyboard::Key::RControl, "Right Control" },
    BindableKey{ Keyboard::Key::RShift, "Right Shift" },
    BindableKey{ Keyboard::Key::RAlt, "Right Alt" },
    BindableKey{ Keyboard::Key::RSystem, "Right System" },
    BindableKey{ Keyboard::Key::Menu, "Menu" },
    BindableKey{ Keyboard::Key::LBracket, "{" },
    BindableKey{ Keyboard::Key::RBracket, "}" },
    BindableKey{ Keyboard::Key::Semicolon, ";" },
    BindableKey{ Keyboard::Key::Comma, "," },
    BindableKey{ Keyboard::Key::Period, "." },
    BindableKey{ Keyboard::Key::Quote, "\"" },
    BindableKey{ Keyboard::Key::Slash, "/" },
    BindableKey{ Keyboard::Key::Backslash, "\\" },
    BindableKey{ Keyboard::Key::Tilde, "~" },
    BindableKey{ Keyboard::Key::Equal, "=" },
    BindableKey{ Keyboard::Key::Hyphen, "-" },
    BindableKey{ Keyboard::Key::Space, "Space" },
    BindableKey{ Keyboard::Key::Enter, "Enter" },
    BindableKey{ Keyboard::Key::Backspace, "Backspace" },
    BindableKey{ Keyboard::Key::Tab, "Tab" },
    BindableKey{ Keyboard::Key::PageUp, "PageUp" },
    BindableKey{ Keyboard::Key::PageDown, "PageDown" },
    BindableKey{ Keyboard::Key::End, "End" },
    BindableKey{ Keyboard::Key::Home, "Home" },
    BindableKey{ Keyboard::Key::Insert, "Insert" },
    BindableKey{ Keyboard::Key::Delete, "Delete" },
    BindableKey{ Keyboard::Key::Add, "Add" },
    BindableKey{ Keyboard::Key::Subtract, "Subtract" },
    BindableKey{ Keyboard::Key::Multiply, "Multiply" },
    BindableKey{ Keyboard::Key::Divide, "Divide" },
    BindableKey{ Keyboard::Key::Left, "Left" },
    BindableKey{ Keyboard::Key::Right, "Right" },
    BindableKey{ Keyboard::Key::Up, "Up" },
    BindableKey{ Keyboard::Key::Down, "Down" },
    BindableKey{ Keyboard::Key::Numpad0, "Numpad0" },
    BindableKey{ Keyboard::Key::Numpad1, "Numpad1" },
    BindableKey{ Keyboard::Key::Numpad2, "Numpad2" },
    BindableKey{ Keyboard::Key::Numpad3, "Numpad3" },
    BindableKey{ Keyboard::Key::Numpad4, "Numpad4" },
    BindableKey{ Keyboard::Key::Numpad5, "Numpad5" },
    BindableKey{ Keyboard::Key::Numpad6, "Numpad6" },
    BindableKey{ Keyboard::Key::Numpad7, "Numpad7" },
    BindableKey{ Keyboard::Key::Numpad8, "Numpad8" },
    BindableKey{ Keyboard::Key::Numpad9, "Numpad9" },
    BindableKey{ Keyboard::Key::F1, "F1" },
    BindableKey{ Keyboard::Key::F2, "F2" },
    BindableKey{ Keyboard::Key::F3, "F3" },
    BindableKey{ Keyboard::Key::F4, "F4" },
    BindableKey{ Keyboard::Key::F5, "F5" },
    BindableKey{ Keyboard::Key::F6, "F6" },
    BindableKey{ Keyboard::Key::F7, "F7" },
    BindableKey{ Keyboard::Key::F8, "F8" },
    BindableKey{ Keyboard::Key::F9, "F9" },
    BindableKey{ Keyboard::Key::F10, "F10" },
    BindableKey{ Keyboard::Key::F11, "F11" },
    BindableKey{ Keyboard::Key::F12, "F12" },
    BindableKey{ Keyboard::Key::F13, "F13" },
    BindableKey{ Keyboard::Key::F14, "F14" },
    BindableKey{ Keyboard::Key::F15, "F15" },
    BindableKey{ Keyboard::Key::Pause, "Pause" },

    // Mouse
    BindableKey{ Mouse::Button::Left, "Left Mouse Button" },
    BindableKey{ Mouse::Button::Right, "Right Mouse Button" },
    BindableKey{ Mouse::Button::Middle, "Middle Mouse Button" },
    BindableKey{ Mouse::Button::XButton1, "X1 Mouse Button" },
    BindableKey{ Mouse::Button::Left, "X2 Mouse Button" },
};

} // namespace hry
