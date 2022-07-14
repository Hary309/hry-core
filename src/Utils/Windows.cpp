#include "Windows.hpp"

#include <Windows.h>

namespace hry
{
void Windows::OpenWebsite(std::string_view url)
{
    ShellExecuteA(nullptr, "open", url.data(), nullptr, nullptr, SW_SHOWNORMAL);
}
}
