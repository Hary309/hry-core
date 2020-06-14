#include <windows.h>
#include <iostream>

#include <hry/Core.hpp>
#include <imgui.h>

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    hry::Foo();

    ImGui::Begin();

    return TRUE;
}


