#include "WndProcEventBridge.hpp"

#include <imgui.h>
#include <windowsx.h>

#include "Hry/Events/Event.hpp"
#include "Hry/System/Keyboard.hpp"
#include "Hry/System/Mouse.hpp"
#include "Hry/System/System.hpp"

#include "Events/EventManager.hpp"
#include "Hooks/D3D11Hook.hpp"

#include "Core.hpp"

IMGUI_IMPL_API LRESULT
    ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

// source: https://github.com/SFML/SFML
// This is a part of SFML under https://github.com/SFML/SFML/blob/master/license.md
Keyboard::Key vkKeyCodeToEnum(WPARAM key, LPARAM flags)
{
    using Key = Keyboard::Key;

    switch (key)
    {
        case VK_SHIFT:
        {
            static UINT lShift = MapVirtualKeyW(VK_LSHIFT, MAPVK_VK_TO_VSC);
            UINT scancode = static_cast<UINT>((flags & (0xFF << 16)) >> 16);
            return scancode == lShift ? Key::LShift : Key::RShift;
        }

        case VK_MENU: return (HIWORD(flags) & KF_EXTENDED) ? Key::RAlt : Key::LAlt;
        case VK_CONTROL: return (HIWORD(flags) & KF_EXTENDED) ? Key::RControl : Key::LControl;

        case 'A': return Key::A;
        case 'Z': return Key::Z;
        case 'E': return Key::E;
        case 'R': return Key::R;
        case 'T': return Key::T;
        case 'Y': return Key::Y;
        case 'U': return Key::U;
        case 'I': return Key::I;
        case 'O': return Key::O;
        case 'P': return Key::P;
        case 'Q': return Key::Q;
        case 'S': return Key::S;
        case 'D': return Key::D;
        case 'F': return Key::F;
        case 'G': return Key::G;
        case 'H': return Key::H;
        case 'J': return Key::J;
        case 'K': return Key::K;
        case 'L': return Key::L;
        case 'M': return Key::M;
        case 'W': return Key::W;
        case 'X': return Key::X;
        case 'C': return Key::C;
        case 'V': return Key::V;
        case 'B': return Key::B;
        case 'N': return Key::N;
        case '0': return Key::Num0;
        case '1': return Key::Num1;
        case '2': return Key::Num2;
        case '3': return Key::Num3;
        case '4': return Key::Num4;
        case '5': return Key::Num5;
        case '6': return Key::Num6;
        case '7': return Key::Num7;
        case '8': return Key::Num8;
        case '9': return Key::Num9;
        case VK_LWIN: return Key::LSystem;
        case VK_RWIN: return Key::RSystem;
        case VK_APPS: return Key::Menu;
        case VK_OEM_1: return Key::Semicolon;
        case VK_OEM_2: return Key::Slash;
        case VK_OEM_PLUS: return Key::Equal;
        case VK_OEM_MINUS: return Key::Hyphen;
        case VK_OEM_4: return Key::LBracket;
        case VK_OEM_6: return Key::RBracket;
        case VK_OEM_COMMA: return Key::Comma;
        case VK_OEM_PERIOD: return Key::Period;
        case VK_OEM_7: return Key::Quote;
        case VK_OEM_5: return Key::Backslash;
        case VK_OEM_3: return Key::Tilde;
        case VK_ESCAPE: return Key::Escape;
        case VK_SPACE: return Key::Space;
        case VK_RETURN: return Key::Enter;
        case VK_BACK: return Key::Backspace;
        case VK_TAB: return Key::Tab;
        case VK_PRIOR: return Key::PageUp;
        case VK_NEXT: return Key::PageDown;
        case VK_END: return Key::End;
        case VK_HOME: return Key::Home;
        case VK_INSERT: return Key::Insert;
        case VK_DELETE: return Key::Delete;
        case VK_ADD: return Key::Add;
        case VK_SUBTRACT: return Key::Subtract;
        case VK_MULTIPLY: return Key::Multiply;
        case VK_DIVIDE: return Key::Divide;
        case VK_LEFT: return Key::Left;
        case VK_RIGHT: return Key::Right;
        case VK_UP: return Key::Up;
        case VK_DOWN: return Key::Down;
        case VK_NUMPAD0: return Key::Numpad0;
        case VK_NUMPAD1: return Key::Numpad1;
        case VK_NUMPAD2: return Key::Numpad2;
        case VK_NUMPAD3: return Key::Numpad3;
        case VK_NUMPAD4: return Key::Numpad4;
        case VK_NUMPAD5: return Key::Numpad5;
        case VK_NUMPAD6: return Key::Numpad6;
        case VK_NUMPAD7: return Key::Numpad7;
        case VK_NUMPAD8: return Key::Numpad8;
        case VK_NUMPAD9: return Key::Numpad9;
        case VK_F1: return Key::F1;
        case VK_F2: return Key::F2;
        case VK_F3: return Key::F3;
        case VK_F4: return Key::F4;
        case VK_F5: return Key::F5;
        case VK_F6: return Key::F6;
        case VK_F7: return Key::F7;
        case VK_F8: return Key::F8;
        case VK_F9: return Key::F9;
        case VK_F10: return Key::F10;
        case VK_F11: return Key::F11;
        case VK_F12: return Key::F12;
        case VK_F13: return Key::F13;
        case VK_F14: return Key::F14;
        case VK_F15: return Key::F15;
        case VK_PAUSE: return Key::Pause;
        case VK_BROWSER_BACK: return Key::BrowserBack;
        case VK_BROWSER_FORWARD: return Key::BrowserForward;
        case VK_BROWSER_REFRESH: return Key::BrowserRefresh;
        case VK_BROWSER_STOP: return Key::BrowserStop;
        case VK_BROWSER_SEARCH: return Key::BrowserSearch;
        case VK_BROWSER_FAVORITES: return Key::BrowserFavourites;
        case VK_BROWSER_HOME: return Key::BrowserHome;
    }

    return Key::Unknown;
}

WndProcEventBridge::WndProcEventBridge(EventManager& eventMgr) : EventBridgeBase(eventMgr)
{
    D3D11Hook::OnWndProc.connect<&WndProcEventBridge::onWndProc>(this);
}

void WndProcEventBridge::onWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);

    switch (msg)
    {
        // window resize
        case WM_SIZE:
        {
            bool shouldSkip = true;
            shouldSkip &= wParam != SIZE_MAXIMIZED;
            shouldSkip &= wParam != SIZE_MINIMIZED;
            shouldSkip &= wParam != SIZE_RESTORED;

            if (shouldSkip)
            {
                break;
            }

            ResizeType resizeType;

            switch (wParam)
            {
                case SIZE_MAXIMIZED: resizeType = ResizeType::Maximized; break;
                case SIZE_MINIMIZED: resizeType = ResizeType::Minimized; break;
                case SIZE_RESTORED: resizeType = ResizeType::Restored; break;
            }

            ResizeEvent resizeEvent{};
            resizeEvent.size.x = LOWORD(lParam);
            resizeEvent.size.y = HIWORD(lParam);
            resizeEvent.type = resizeType;

            _eventMgr.windowResizeSignal.call(std::move(resizeEvent));
        }
        break;

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            if ((HIWORD(lParam) & KF_REPEAT) == 0)
            {
                Core::Logger->info("Key pressed ", wParam, " ", lParam);
                _eventMgr.keyPressSignal.call(
                    KeyboardEvent{ vkKeyCodeToEnum(wParam, lParam), ButtonState::Pressed });
            }
        }
        break;

        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            _eventMgr.keyReleaseSignal.call(
                KeyboardEvent{ vkKeyCodeToEnum(wParam, lParam), ButtonState::Released });
        }
        break;

        case WM_MOUSEWHEEL:
        {
            MouseWheelEvent wheelEvent{};
            wheelEvent.wheel = Mouse::Wheel::Vertical;
            wheelEvent.delta = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;

            _eventMgr.mouseWheelScrollSignal.call(std::move(wheelEvent));
        }
        break;

        // mouse wheel
        case WM_MOUSEHWHEEL:
        {
            MouseWheelEvent wheelEvent{};
            wheelEvent.wheel = Mouse::Wheel::Horizontal;
            wheelEvent.delta = GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;

            _eventMgr.mouseWheelScrollSignal.call(std::move(wheelEvent));
        }
        break;

        case WM_MOUSEMOVE:
        {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);

            MouseMoveEvent moveEvent;
            moveEvent.offset.x = x - _lastMousePos.x;
            moveEvent.offset.y = y - _lastMousePos.y;

            _lastMousePos = { x, y };

            _eventMgr.mouseMoveSignal.call(std::move(moveEvent));
        }
        break;

        // Left mouse button
        case WM_LBUTTONDOWN:
        {
            _eventMgr.mouseButtonPressSignal.call({ Mouse::Button::Left, ButtonState::Pressed });
        }
        break;
        case WM_LBUTTONUP:
        {
            _eventMgr.mouseButtonReleaseSignal.call({ Mouse::Button::Left, ButtonState::Released });
        }
        break;

        // Right mouse button
        case WM_RBUTTONDOWN:
        {
            _eventMgr.mouseButtonPressSignal.call({ Mouse::Button::Right, ButtonState::Pressed });
        }
        break;
        case WM_RBUTTONUP:
        {
            _eventMgr.mouseButtonReleaseSignal.call(
                { Mouse::Button::Right, ButtonState::Released });
        }
        break;

        // Middle mouse button
        case WM_MBUTTONDOWN:
        {
            _eventMgr.mouseButtonPressSignal.call({ Mouse::Button::Middle, ButtonState::Pressed });
        }
        break;
        case WM_MBUTTONUP:
        {
            _eventMgr.mouseButtonReleaseSignal.call(
                { Mouse::Button::Middle, ButtonState::Released });
        }
        break;

        // xbutton
        case WM_XBUTTONDOWN:
        {
            Mouse::Button button = GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ?
                                       Mouse::Button::XButton1 :
                                       Mouse::Button::XButton2;

            _eventMgr.mouseButtonPressSignal.call({ button, ButtonState::Pressed });
        }
        break;
        case WM_XBUTTONUP:
        {
            Mouse::Button button = GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ?
                                       Mouse::Button::XButton1 :
                                       Mouse::Button::XButton2;

            _eventMgr.mouseButtonReleaseSignal.call({ button, ButtonState::Released });
        }
        break;
    }
}

HRY_NS_END
