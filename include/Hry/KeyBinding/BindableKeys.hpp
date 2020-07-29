#pragma once

#include <array>
#include <string>
#include <variant>

#include "Hry/Export.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/System/Keyboard.hpp"
#include "Hry/System/Mouse.hpp"

HRY_NS_BEGIN

struct BindableKey
{
    using Key_t = std::variant<Keyboard::Key, Mouse::Button>; // TODO: Add controller's keys

    Key_t key;
    std::string name;

    template<typename T>
    [[nodiscard]] bool has() const
    {
        return std::holds_alternative<T>(key);
    }
};

HRY_API const BindableKey* GetBindableKey(BindableKey::Key_t key);

HRY_NS_END
