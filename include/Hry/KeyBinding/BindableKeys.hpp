/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <array>
#include <string>
#include <variant>

#include "Hry/Export.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/System/Joystick.hpp"
#include "Hry/System/Keyboard.hpp"
#include "Hry/System/Mouse.hpp"

HRY_NS_BEGIN

struct BindableKey final
{
    using Key_t = std::variant<Keyboard::Key, Mouse::Button, Joystick::Button>;

    Key_t key;
    std::string_view name;

    template<typename T>
    [[nodiscard]] bool has() const
    {
        return std::holds_alternative<T>(key);
    }
};

HRY_API const BindableKey* GetBindableKey(BindableKey::Key_t key);

HRY_NS_END
