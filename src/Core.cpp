#include "Core.hpp"

#include <cstdio>
#include <string>

#include <imgui.h>
#include <scssdk_telemetry.h>

#include "Hry/Config/ConfigFieldBase.hpp"
#include "Hry/Config/Fields/BoolField.hpp"
#include "Hry/Config/Fields/SelectionField.hpp"
#include "Hry/Config/Fields/TextField.hpp"
#include "Hry/Events/Event.hpp"
#include "Hry/Events/EventHandler.hpp"
#include "Hry/Namespace.hpp"
#include "Hry/Utils/Signal.hpp"

#include "Hooks/D3D11Hook.hpp"
#include "Hooks/DInput8Hook.hpp"
#include "Logger/LoggerFactory.hpp"
#include "Utils/ImGuiUtils.hpp"

HRY_NS_BEGIN

Core::Core(HINSTANCE hInst)
    : _renderer(*this), _eventHandler(_eventMgr.createEventHandler()), _keyBindsMgr(_eventHandler),
      _moduleMgr("plugins\\hry_plugins", _eventMgr, _configMgr, _keyBindsMgr),
      _mainWindow(_moduleMgr, _configMgr, _keyBindsMgr, _eventHandler),
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

#ifdef DEBUG
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stdin);
#endif

    LoggerFactory::Init("plugins/hry_core.log");
    Logger = LoggerFactory::GetLogger("core");

    Logger->info("Initializing core...");

    bool success = true;

    success &= Core::InstallHooks();

    _renderer.init();
    _eventMgr.init(scsTelemetry);

    _isInited = true;

    return success;
}

void Core::lateInit()
{
    EnableImGui(false);
    initConfig();
    initKeyBinds();

    _moduleMgr.init();

    Logger->info("Core successfully initialized!");
}

void Core::initConfig()
{
    _coreConfig = _configMgr.createConfig("Core");
    _coreConfig->onChangesApplied.connect(
        [](void* /*unused*/, ConfigCallbackData&& callbackData) {
            auto test = callbackData.getValue<bool>("test");
            auto text = callbackData.getValue<std::string>("text");
            auto selectOne = callbackData.getValue<std::string>("selectOne");

            if (test.has_value() && text.has_value() && selectOne.has_value())
            {
                Core::Logger->info(
                    "Config values: '", test.value(), "', '", text.value(), "', '",
                    selectOne.value(), "'");
            }
        },
        nullptr);

    auto* checkBox = _coreConfig->createField<BoolField>("Test", "test");
    checkBox->setDefaultValue(false);
    checkBox->setDescription("some desc");

    auto* text = _coreConfig->createField<TextField>("Tekst", "text");
    text->setDefaultValue("Extra tekst");

    auto* select = _coreConfig->createField<SelectionField>("Wybierz se", "select_one");
    select->addOptions("One", "Two", "Three");
    select->setDefaultValue("Two");
    select->useCombo();

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
    _mainWindow.renderImGui();

    _eventMgr.imguiRenderSignal.call();
}

bool Core::InstallHooks()
{
    Logger->info("Installing hooks...");

    bool success = true;

    success &= D3D11Hook::Install();
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
    DInput8Hook::Uninstall();

    Logger->info("Hooks uninstalled");
}

HRY_NS_END
