#include "Windows.hpp"

#include <Windows.h>

HRY_NS_BEGIN

void Windows::OpenWebsite(std::string_view url)
{
    ShellExecuteA(nullptr, "open", url.data(), nullptr, nullptr, SW_SHOWNORMAL);
}

HRY_NS_END
