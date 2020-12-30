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

/**
 * @brief Struct for holding possible keys to bind
 */
struct BindableKey final
{
    /**
     * @brief Possible bindable key type
     * 
     * Possible to bind are:
     * - Keyboard keys 
     * - Mouse buttons
     * - Joystick buttons
     */
    using Key_t = std::variant<Keyboard::Key, Mouse::Button, Joystick::Button>;

    /**
     * @brief Key possible to bind
     */
    Key_t key;
    /**
     * @brief Name of the key
     */
    std::string_view name;

    /**
     * @brief Check if structure hold specified bindable key type
     * 
     * @tparam T Bindable key type in Key_t
     * @return true if has
     */
    template<typename T>
    [[nodiscard]] bool has() const
    {
        return std::holds_alternative<T>(key);
    }
};

/**
 * @brief Get the Bindable Key for specified key
 * 
 * Returned keys will never be nullptr
 * 
 * @param key Key to convert to bindable key
 * @return Bindable key, nullptr if not found
 */
HRY_API const BindableKey* GetBindableKey(BindableKey::Key_t key);

HRY_NS_END
