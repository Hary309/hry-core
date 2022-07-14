/*
 * This file is part of the CitizenFX project - http://citizen.re/
 *
 * See LICENSE and MENTIONS in the root of https://github.com/citizenfx/fivem/ for information
 * regarding licensing.
 */

#include "Hry/Memory/Hooking.Patterns.hpp"

#include <windows.h>

#include <algorithm>
#include <string_view>

#undef max

namespace hry
{
namespace detail
{
void set_base()
{
    detail::set_base((uintptr_t)GetModuleHandle(nullptr));
}
}

static void TransformPattern(std::string_view pattern, std::string& data, std::string& mask)
{
    uint8_t tempDigit = 0;
    bool tempFlag = false;

    auto tol = [](char ch) -> uint8_t {
        if (ch >= 'A' && ch <= 'F')
            return uint8_t(ch - 'A' + 10);
        if (ch >= 'a' && ch <= 'f')
            return uint8_t(ch - 'a' + 10);
        return uint8_t(ch - '0');
    };

    for (auto ch : pattern)
    {
        if (ch == ' ')
        {
            continue;
        }

        if (ch == '?')
        {
            data.push_back(0);
            mask.push_back('?');
        }
        else if (
            (ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f'))
        {
            uint8_t thisDigit = tol(ch);

            if (!tempFlag)
            {
                tempDigit = thisDigit << 4;
                tempFlag = true;
            }
            else
            {
                tempDigit |= thisDigit;
                tempFlag = false;

                data.push_back(tempDigit);
                mask.push_back('x');
            }
        }
    }
}

class executable_meta
{
private:
    uintptr_t m_begin;
    uintptr_t m_end;

public:
    template<typename TReturn, typename TOffset>
    TReturn* getRVA(TOffset rva)
    {
        return (TReturn*)(m_begin + rva);
    }

    explicit executable_meta(void* module)
        : m_begin((uintptr_t)module)
    {
        PIMAGE_DOS_HEADER dosHeader = getRVA<IMAGE_DOS_HEADER>(0);
        PIMAGE_NT_HEADERS ntHeader = getRVA<IMAGE_NT_HEADERS>(dosHeader->e_lfanew);

        m_end = m_begin + ntHeader->OptionalHeader.SizeOfCode;
    }

    executable_meta(uintptr_t begin, uintptr_t end)
        : m_begin(begin)
        , m_end(end)
    {
    }

    inline uintptr_t begin() const { return m_begin; }
    inline uintptr_t end() const { return m_end; }
};

void pattern::Initialize(const char* pattern, size_t length)
{
    // transform the base pattern from IDA format to canonical format
    TransformPattern(std::string_view(pattern, length), m_bytes, m_mask);
}

void pattern::EnsureMatches(uint32_t maxCount)
{
    if (m_matched)
    {
        return;
    }

    // scan the executable for code
    executable_meta executable = m_rangeStart != 0 && m_rangeEnd != 0 ?
                                     executable_meta(m_rangeStart, m_rangeEnd) :
                                     executable_meta(m_module);

    auto matchSuccess = [&](uintptr_t /*address*/) {
        return (m_matches.size() == maxCount);
    };

    const uint8_t* pattern = reinterpret_cast<const uint8_t*>(m_bytes.c_str());
    const char* mask = m_mask.c_str();
    size_t maskSize = m_mask.size();
    size_t lastWild = m_mask.find_last_of('?');

    ptrdiff_t Last[256];

    std::fill(
        std::begin(Last), std::end(Last), lastWild == std::string::npos ? -1 : static_cast<ptrdiff_t>(lastWild));

    for (ptrdiff_t i = 0; i < static_cast<ptrdiff_t>(maskSize); ++i)
    {
        if (Last[pattern[i]] < i)
        {
            Last[pattern[i]] = i;
        }
    }

    for (uintptr_t i = executable.begin(), end = executable.end() - maskSize; i <= end;)
    {
        uint8_t* ptr = reinterpret_cast<uint8_t*>(i);
        ptrdiff_t j = maskSize - 1;

        while ((j >= 0) && (mask[j] == '?' || pattern[j] == ptr[j]))
            j--;

        if (j < 0)
        {
            m_matches.emplace_back(ptr);

            if (matchSuccess(i))
            {
                break;
            }
            i++;
        }
        else
            i += std::max((ptrdiff_t)1, j - Last[ptr[j]]);
    }

    m_matched = true;
}

bool pattern::ConsiderMatch(uintptr_t offset)
{
    const char* pattern = m_bytes.c_str();
    const char* mask = m_mask.c_str();

    char* ptr = reinterpret_cast<char*>(offset);

    for (size_t i = 0, j = m_mask.size(); i < j; i++)
    {
        if (mask[i] == '?')
        {
            continue;
        }

        if (pattern[i] != ptr[i])
        {
            return false;
        }
    }

    m_matches.emplace_back(ptr);

    return true;
}
}