/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#pragma once

#include <algorithm>
#include <memory>

#include "Hry/Config/Config.hpp"
#include "Hry/GameType.hpp"
#include "Hry/Logger/Logger.hpp"
#include "Hry/SCSSDK/Telemetry.hpp"
#include "Hry/Utils/Delegate.hpp"
#include "Hry/Utils/Utils.hpp"

#include "Events/EventDispatcher.hpp"
#include "KeyBinding/KeyBinds.hpp"

#include "PluginInfo.hpp"

namespace hry
{
/**
 * @brief Base class for plugin
 */
class Plugin
{
public:
    /**
     * @brief Values returned to API about the status of plugin
     */
    enum class Result
    {
        Ok,               // plugin initialized successfully
        ApiNotSupported,  // api version not supported (incompatible, abi breaks)
        GameNotSupported, // game version not supported (e.g. byte pattern don't match)
        Error             // plugin have internal error
    };

    /**
     * @brief Parameters passed to plugin by API
     */
    struct InitParams
    {
        Logger* logger;
        const Telemetry* telemetry;
        Version apiVersion;
        GameType gameType;
    };

public:
    Version ApiVersion = hry::ApiVersion;

public:
    Plugin() = default;
    Plugin(Plugin&&) = delete;
    Plugin(const Plugin&) = delete;
    Plugin& operator=(Plugin&&) = delete;
    Plugin& operator=(const Plugin&) = delete;
    virtual ~Plugin() = default;

    /**
     * @brief Interface for general initialization
     *        Is called as first
     *
     * @param initParams Holds useful information
     *                   pointers are freed after destructor call
     * @return Information to API about plugin status
     *         Only for Result::Ok plugin won't be unloaded
     */
    virtual Result init(const InitParams&& initParams) = 0;

    /**
     * @brief Interface for initalizing config
     *        Is called as third
     *
     * @param config Holds class for creating config
     */
    virtual void initConfig(Config* config) = 0;

    /**
     * @brief Interface for initializing key bindings
     *        Is called as fourth
     *
     * @param keyBinds Holds class for creating key bindings
     */
    virtual void initKeyBinds(KeyBinds* keyBinds) = 0;

    /**
     * @brief Interface for registering events
     *        Is called as second
     *
     * @param eventDispatcher Holds structure for registering for event calls
     */
    virtual void initEvents(EventDispatcher* eventDispatcher) = 0;

    /**
     * @brief Intarface for getting information about plugin
     *
     * @todo replace with manifest.json or sth
     *
     * @return reference for plugin info structure
     */
    [[nodiscard]] virtual const PluginInfo& getPluginInfo() const = 0;
};
}

/**
 * @brief Simple macro for initializing plugin
 *        Put it once in main file with class that inherits from Plugin
 */
#define INIT_PLUGIN(PLUGIN_TYPE)                                                                   \
    extern "C"                                                                                     \
    {                                                                                              \
        __declspec(dllexport) hry::Plugin* CreatePlugin() { return new PLUGIN_TYPE(); }            \
    }
