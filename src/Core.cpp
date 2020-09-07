#include "Core.hpp"

#include <cstdio>
#include <filesystem>
#include <ios>
#include <string>

#include <amtrucks/scssdk_ats.h>
#include <eurotrucks2/scssdk_eut2.h>
#include <imgui.h>
#include <scssdk_telemetry.h>
#include <vcruntime_typeinfo.h>

#include "Hry/Config/ConfigFieldBase.hpp"
#include "Hry/Config/Fields/BoolField.hpp"
#include "Hry/Config/Fields/SelectionField.hpp"
#include "Hry/Config/Fields/TextField.hpp"
#include "Hry/Events/Event.hpp"
#include "Hry/Events/EventHandler.hpp"
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
    : _eventHandler(_eventMgr.createEventHandler()), _renderer(*this, _eventMgr),
      _keyBindsMgr(_eventHandler), _moduleMgr(_eventMgr, _configMgr, _keyBindsMgr),
      _mainWindow(_moduleMgr, _configMgr, _keyBindsMgr, _eventHandler), _loggerWindow(_eventMgr),
      _imguiImplEvents(_eventHandler)
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
    _scsTelemetry = scsTelemetry;

    Paths::Init();

    Core::GameType = DetermineGameType(_scsTelemetry->common.game_id);

    LoggerFactory::Init(Paths::HomePath + "/hry_core.log", _eventMgr);
    Logger = LoggerFactory::GetLogger("core");

    Logger->info("Base address: {0:#x}", GetBaseAddress());
    Logger->info("API version {}.{}.{}", ApiVersion.major, ApiVersion.minor, ApiVersion.patch);
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

    _isInited = true;

    scsTelemetry->common.log(SCS_LOG_TYPE_message, "[hry-core] Initialized!");

    return success;
}

void Core::lateInit()
{
    InternalImGuiUtils::LoadFonts();
    InternalImGuiUtils::ApplyDarkTheme();
    InternalImGuiUtils::EnableCursor(false);

    initConfig();
    initKeyBinds();

    _moduleMgr.init();

    Logger->info("Core successfully initialized!");
}

void Core::initConfig()
{
    _coreConfig = _configMgr.createConfig("Core");
    _coreConfig->setBindingType<CoreConfig>();
    _coreConfig->onChangesApplied.connect<&Core::onConfigChangesApplied>(this);

    auto* showLogWindow = _coreConfig->createField<BoolField>("Show log window", "show_log_window");
    showLogWindow->bind(&CoreConfig::showLogWindow);
    showLogWindow->setDefaultValue(false);

    auto* showImGuiDemo = _coreConfig->createField<BoolField>("Show ImGUI Demo", "show_imgui_demo");
    showImGuiDemo->bind(&CoreConfig::showImGuiDemo);
    showImGuiDemo->setDefaultValue(false);

    _configMgr.loadFor(_coreConfig.get());
}

void Core::initKeyBinds()
{
    _coreKeyBinds = _keyBindsMgr.createKeyBinds("Core");
    _mainWindow.initKeyBinds(*_coreKeyBinds);
    _keyBindsMgr.loadFor(_coreKeyBinds.get());
}

void Core::imguiRender()
{
    if (_showImGuiDemo)
    {
        ImGui::ShowDemoWindow();
    }

    _mainWindow.imguiRender();
    _loggerWindow.imguiRender();

    _eventMgr.imguiRenderSignal.call();
}

void Core::onConfigChangesApplied(const ConfigCallbackData& data)
{
    const auto* coreConfigData = data.getData<CoreConfig>();

    _loggerWindow.setEnabled(coreConfigData->showLogWindow);
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
