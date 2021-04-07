/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 */

#include "Core.hpp"

#include <cstdio>
#include <filesystem>
#include <string>

#include <amtrucks/scssdk_ats.h>
#include <eurotrucks2/scssdk_eut2.h>
#include <imgui.h>
#include <scssdk_telemetry.h>

#include "Hry/Config/ConfigFieldBase.hpp"
#include "Hry/Config/Fields/BoolField.hpp"
#include "Hry/Config/Fields/NumericField.hpp"
#include "Hry/Config/Fields/SelectionField.hpp"
#include "Hry/Config/Fields/TextField.hpp"
#include "Hry/Events/Event.hpp"
#include "Hry/Events/EventDispatcher.hpp"
#include "Hry/GameType.hpp"
#include "Hry/Memory/Memory.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/Utils/Paths.hpp"
#include "Hry/Utils/Signal.hpp"
#include "Hry/Utils/Utils.hpp"
#include "Hry/Version.hpp"

#include "Hooks/D3D11Hook.hpp"
#include "Hooks/DInput8Hook.hpp"
#include "Hooks/OpenGLHook.hpp"
#include "Logger/LoggerFactory.hpp"
#include "Utils/InternalImGuiUtils.hpp"

HRY_NS_BEGIN

Core::Core(HINSTANCE hInst)
    : _eventDispatcher(_eventMgr.createInternalEventDispatcher()),
      _channelAggregator(_eventDispatcher), _renderer(*this, _eventMgr),
      _keyBindsMgr(_eventDispatcher),
      _moduleMgr(_eventMgr, _configMgr, _keyBindsMgr, _channelAggregator.getTelemetry()),
      _mainWindow(_moduleMgr, _configMgr, _keyBindsMgr, _eventMgr, _eventDispatcher),
      _loggerWindow(_eventDispatcher), _imguiImplEvents(_eventDispatcher)
{
    hInstance = hInst;
}

Core::~Core()
{
    Logger->info("Deinitializing...");
    Core::UninstallHooks();
}

bool Core::init(scs_telemetry_init_params_v100_t* scsTelemetry)
{
    Core::GameType = DetermineGameType(scsTelemetry->common.game_id);

    Paths::Init();
    ImGui::GetIO().IniFilename = _strdup((Paths::HomePath + "\\imgui.ini").c_str());

    LoggerFactory::Init(Paths::HomePath + "/hry_core.log", _eventMgr);
    Logger = LoggerFactory::GetLogger("core");

    Logger->info("Base address: 0x{:X}", GetBaseAddress());
    Logger->info("API version {}", ApiVersion);
    Logger->info("Game name: {}", GameTypeToString(Core::GameType));
    Logger->info("Module's path: {}", Paths::ModulePath);
    Logger->info("Home path: {}", Paths::HomePath);

    bool success = true;

    success &= Core::InstallHooks();

    if (success == false)
    {
        scsTelemetry->common.log(SCS_LOG_TYPE_error, "[hry-core] Cannot initialize!");
        Logger->error("Cannot initialize!");
        Core::UninstallHooks();
        return false;
    }

    _renderer.init();
    _eventMgr.init(scsTelemetry);
    _channelAggregator.init(scsTelemetry);

    initConfig();
    initKeyBinds();

    _moduleMgr.init();

    _isInited = true;

    scsTelemetry->common.log(SCS_LOG_TYPE_message, "[hry-core] Initialized!");

    return success;
}

void Core::lateInit()
{
    InternalImGuiUtils::LoadFonts();
    InternalImGuiUtils::ApplyDarkTheme();
    InternalImGuiUtils::EnableCursor(false);

    Logger->info("Core successfully initialized!");
}

void Core::initConfig()
{
    _coreConfig = _configMgr.createConfig("Core");
    _coreConfig->setBindingType<CoreConfig>();
    _coreConfig->onChangesApplied.connect<&Core::onConfigChangesApplied>(this);

    _coreConfig->add(BoolFieldBuilder()
                         .setID("show_imgui_demo")
                         .setLabel("Show ImGui Demo")
                         .setDescription("Developer option")
                         .bind(&CoreConfig::showImGuiDemo)
                         .setDefaultValue(false)
                         .build());

    _coreConfig->add(BoolFieldBuilder()
                         .setID("show_log_window")
                         .setLabel("Show log window")
                         .setDescription("Developer option")
                         .bind(&CoreConfig::showLogWindow)
                         .setDefaultValue(false)
                         .build());

    _coreConfig->add(NumericFieldBuilder<float>()
                         .setID("log_window_opacity")
                         .setLabel("Log window opacity")
                         .setDescription("Developer option")
                         .bind(&CoreConfig::logWindowOpacity)
                         .setDefaultValue(0.94f)
                         .useSlider(0, 1, "%.2f")
                         .build());

    if (!_coreConfig->loadFromFile())
    {
        _coreConfig->saveToFile();
    }
}

void Core::initKeyBinds()
{
    _coreKeyBinds = _keyBindsMgr.createKeyBinds("Core");
    _mainWindow.initKeyBinds(*_coreKeyBinds);

    if (!_coreKeyBinds->loadFromFile())
    {
        _coreKeyBinds->saveToFile();
    }
}

void Core::imguiRender()
{
    if (_showImGuiDemo)
    {
        ImGui::ShowDemoWindow();
    }

    _mainWindow.imguiRender();
    _loggerWindow.imguiRender();

    _eventMgr.system.imguiRenderSignal.call();
}

void Core::onConfigChangesApplied(const ConfigCallbackData& data)
{
    const auto* coreConfigData = data.getData<CoreConfig>();

    _loggerWindow.setEnabled(coreConfigData->showLogWindow);
    _loggerWindow.setOpacity(coreConfigData->logWindowOpacity);

    _showImGuiDemo = coreConfigData->showImGuiDemo;
}

bool Core::InstallHooks()
{
    bool success = true;

    bool rendererSuccess = false;

    rendererSuccess |= D3D11Hook::Install();
    rendererSuccess |= OpenGLHook::Install();

    success &= rendererSuccess;

    success &= DInput8Hook::Install();

    if (success)
    {
        Logger->info("Hooks installed");
    }
    else
    {
        Logger->error("Cannot install hooks");
    }

    return success;
}

void Core::UninstallHooks()
{
    Logger->info("Uninstalling hooks...");

    D3D11Hook::Uninstall();
    OpenGLHook::Uninstall();
    DInput8Hook::Uninstall();

    Logger->info("Hooks uninstalled");
}

GameType Core::DetermineGameType(const char* gameID)
{
    if (strcmp(gameID, SCS_GAME_ID_EUT2) == 0)
    {
        return GameType::ETS2;
    }

    if (strcmp(gameID, SCS_GAME_ID_ATS) == 0)
    {
        return GameType::ATS;
    }

    return GameType::Unknown;
}

HRY_NS_END
