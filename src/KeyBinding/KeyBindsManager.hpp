#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <vector>

#include "Hry/Events/Event.hpp"
#include "Hry/Events/EventHandler.hpp"
#include "Hry/KeyBinding/BindableKeys.hpp"
#include "Hry/KeyBinding/KeyBinds.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/System/System.hpp"
#include "Hry/Utils/Delegate.hpp"
#include "Hry/Utils/TaskScheduler.hpp"
#include "Hry/Utils/Utils.hpp"

#include "Events/EventManager.hpp"

HRY_NS_BEGIN

class KeyBindsManager
{
public:
    inline static constexpr auto ConfigDirectory = "plugins/hry_config";
    inline static constexpr auto FilePath = "plugins/hry_config/keybinds.json";

    inline static constexpr auto LongPressTimeout = std::chrono::milliseconds(400);

private:
    std::vector<KeyBinds*> _keyBinds;
    TaskScheduler<void(KeyBind*, std::chrono::system_clock::time_point)> _taskScheduler;

public:
    explicit KeyBindsManager(EventHandler& eventHandler);

    DelegateDeleterUniquePtr_t<KeyBinds> createKeyBinds(const std::string& name);
    void remove(const KeyBinds* keyBind);

    void update();

    [[nodiscard]] const auto& getKeyBinds() const { return _keyBinds; }

    void save();
    void loadFor(KeyBinds* keyBinds);

private:
    void keyBindsDeleter(KeyBinds* ptr);

    void handleKeybaordEvent(const KeyboardEvent&& keyboardEvent);
    void handleMouseButtonEvent(const MouseButtonEvent&& buttonEvent);
    void handleJoystickButtonEvent(const JoystickButtonEvent&& buttonEvent);

    void processKey(BindableKey::Key_t key, ButtonState buttonState, std::optional<GUID> guid);

    static void handleClickActivator(ButtonState buttonState, KeyBind* keyBind);
    void handleHoldActivator(ButtonState buttonState, KeyBind* keyBind);

    void onTaskHold(KeyBind* keyBind, std::chrono::system_clock::time_point);
};

HRY_NS_END