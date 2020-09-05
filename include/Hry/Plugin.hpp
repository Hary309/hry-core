#pragma once

#include <algorithm>
#include <memory>

#include "Hry/Config/Config.hpp"
#include "Hry/Logger/Logger.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/Utils.hpp"
#include "Hry/Utils/Delegate.hpp"

#include "Events/EventHandler.hpp"
#include "KeyBinding/KeyBinds.hpp"

#include "PluginInfo.hpp"

HRY_NS_BEGIN

class Plugin
{
public:
    enum class Result
    {
        Ok,               // plugin initialized successfully
        ApiNotSupported,  // api version not supported (incompatible, abi breaks)
        GameNotSupported, // game version not supported
        Error             // plugin have internal error
    };

    struct InitParams
    {
        Logger* logger;
        Version apiVersion;
        Version gameVersion;
    };

public:
    Plugin() = default;
    Plugin(Plugin&&) = delete;
    Plugin(const Plugin&) = delete;
    Plugin& operator=(Plugin&&) = delete;
    Plugin& operator=(const Plugin&) = delete;
    virtual ~Plugin() = default;

    virtual Result init(const InitParams&& initParams) = 0;
    virtual void initConfig(Config* config) = 0;
    virtual void initKeyBinds(KeyBinds* keyBinds) = 0;
    virtual void initEvents(EventHandler* eventHandler) = 0;

    virtual void imguiPage() = 0;

    // replace with manifest.json
    [[nodiscard]] virtual const PluginInfo& getPluginInfo() const = 0;
};

HRY_NS_END

#define INIT_PLUGIN(PLUGIN_TYPE)                                                                   \
    extern "C"                                                                                     \
    {                                                                                              \
        __declspec(dllexport) hry::Plugin* CreatePlugin() { return new PLUGIN_TYPE(); }            \
    }
