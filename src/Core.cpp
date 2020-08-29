#include "Core.hpp"

#include <cstdio>
#include <ios>
#include <string>

#include <imgui.h>
#include <scssdk_telemetry.h>

#include "Hry/Config/ConfigFieldBase.hpp"
#include "Hry/Config/Fields/BoolField.hpp"
#include "Hry/Config/Fields/SelectionField.hpp"
#include "Hry/Config/Fields/TextField.hpp"
#include "Hry/Events/Event.hpp"
#include "Hry/Events/EventHandler.hpp"
#include "Hry/Memory/Memory.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/Utils/Signal.hpp"

#include "Hooks/D3D11Hook.hpp"
#include "Hooks/DInput8Hook.hpp"
#include "Hooks/OpenGLHook.hpp"
#include "Logger/LoggerFactory.hpp"
#include "Utils/ImGuiUtils.hpp"

HRY_NS_BEGIN

Core::Core(HINSTANCE hInst)
    : _eventHandler(_eventMgr.createEventHandler()), _renderer(*this, _eventMgr),
      _keyBindsMgr(_eventHandler),
      _moduleMgr("plugins\\hry_plugins", _eventMgr, _configMgr, _keyBindsMgr),
      _mainWindow(_moduleMgr, _configMgr, _keyBindsMgr, _eventHandler), _loggerWindow(_eventMgr),
      _imguiImplEvents(_eventHandler)
{
    hInstance = hInst;
}

Core::~Core()
{
    Logger->info("Deinitializing...");
    _moduleMgr.unloadAll();
    Core::UninstallHooks();
}

bool Core::init(scs_telemetry_init_params_v100_t* scsTelemetry)
{
    _scsTelemetry = scsTelemetry;

    LoggerFactory::Init("plugins/hry_core.log", _eventMgr);
    Logger = LoggerFactory::GetLogger("core");

    Logger->info("Base address: {0:#x}", GetBaseAddress());

    Logger->info("Initializing core...");

    bool success = true;

    success &= Core::InstallHooks();

    if (success == false)
    {
        Logger->info("Cannot initialize!");
        Core::UninstallHooks();
        return false;
    }

    _renderer.init();
    _eventMgr.init(scsTelemetry);

    _isInited = true;

    return success;
}

void Core::lateInit()
{
    EnableImGuiCursor(false);
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

    auto* checkBox = _coreConfig->createField<BoolField>("Show log window", "show_log_window");
    checkBox->bind(&CoreConfig::showLogWindow);
    checkBox->setDefaultValue(false);

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
    ImGui::ShowDemoWindow();

    _mainWindow.imguiRender();
    _loggerWindow.imguiRender();

    _eventMgr.imguiRenderSignal.call();
}

void Core::onConfigChangesApplied(const ConfigCallbackData& data)
{
    const auto* coreConfigData = data.getData<CoreConfig>();

    _loggerWindow.setEnabled(coreConfigData->showLogWindow);
}

bool Core::InstallHooks()
{
    Logger->info("Installing hooks...");

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

HRY_NS_END
