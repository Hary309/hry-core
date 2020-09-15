/*
 * This file is part of the CitizenFX project - http://citizen.re/
 *
 * See LICENSE and MENTIONS in the root of https://github.com/citizenfx/fivem/ for information
 * regarding licensing.
 */

#pragma once
#include <cassert>
#include <string>
#include <vector>

#include "Hry/Export.hpp"

#pragma warning(push)
#pragma warning(disable : 4201)

namespace hry
{
    namespace detail
    {
        inline ptrdiff_t baseAddressDifference;

        // sets the base address difference based on an obtained pointer
        inline void set_base(uintptr_t address)
        {
#ifdef _M_IX86
            uintptr_t addressDiff = (address - 0x400000);
#elif defined(_M_AMD64)
            uintptr_t addressDiff = (address - 0x140000000);
#endif

            // pointer-style cast to ensure unsigned overflow ends up copied directly into a signed value
            baseAddressDifference = *(ptrdiff_t*)&addressDiff;
        }

        HRY_API void set_base();

        template<typename T>
        inline T* getRVA(uintptr_t rva)
        {
#ifdef _M_IX86
            return (T*)(baseAddressDifference + 0x400000 + rva);
#elif defined(_M_AMD64)
            return (T*)(baseAddressDifference + 0x140000000 + rva);
#endif
        }

    } // namespace detail

    class HRY_API pattern_match
    {
    private:
        void* m_pointer;

    public:
        inline pattern_match(void* pointer) : m_pointer(pointer) {}

        template<typename T>
        T* get(ptrdiff_t offset = 0) const
        {
            char* ptr = reinterpret_cast<char*>(m_pointer);
            return reinterpret_cast<T*>(ptr + offset);
        }
    };

    class HRY_API pattern
    {
    private:
        std::string m_bytes;
        std::string m_mask;

        std::vector<pattern_match> m_matches;

        bool m_matched;

        union
        {
            void* m_module;
            struct
            {
                uintptr_t m_rangeStart;
                uintptr_t m_rangeEnd;
            };
        };

    protected:
        inline pattern(void* module)
            : m_rangeStart((uintptr_t)module), m_matched(false), m_rangeEnd(0)
        {
        }

        inline pattern(uintptr_t begin, uintptr_t end)
            : m_rangeStart(begin), m_rangeEnd(end), m_matched(false)
        {
        }

        void Initialize(const char* pattern, size_t length);

    private:
        bool ConsiderMatch(uintptr_t offset);

        void EnsureMatches(uint32_t maxCount);

        inline pattern_match _get_internal(size_t index) const { return m_matches[index]; }

    public:
        template<size_t Len>
        pattern(const char (&p)[Len]) : pattern(detail::getRVA<void>(0))
        {
            Initialize(p, Len);
        }

        pattern(std::string_view p) : pattern(detail::getRVA<void>(0))
        {
            Initialize(p.data(), p.size());
        }

        inline pattern& count(uint32_t expected) &
        {
            EnsureMatches(expected);
            assert(m_matches.size() == expected);
            return *this;
        }

        inline pattern& count_hint(uint32_t expected) &
        {
            EnsureMatches(expected);
            return *this;
        }

        inline pattern& clear() &
        {
            m_matches.clear();
            m_matched = false;
            return *this;
        }

        inline pattern&& count(uint32_t expected) &&
        {
            EnsureMatches(expected);
            assert(m_matches.size() == expected);
            return std::move(*this);
        }

        inline pattern&& count_hint(uint32_t expected) &&
        {
            EnsureMatches(expected);
            return std::move(*this);
        }

        inline pattern&& clear() &&
        {
            m_matches.clear();
            m_matched = false;
            return std::move(*this);
        }

        inline size_t size()
        {
            EnsureMatches(UINT32_MAX);
            return m_matches.size();
        }

        inline bool empty() { return size() == 0; }

        inline pattern_match get(size_t index)
        {
            EnsureMatches(UINT32_MAX);
            return _get_internal(index);
        }

        inline pattern_match get_one()
        {
            return std::forward<pattern>(*this).count(1)._get_internal(0);
        }

        template<typename T = void>
        inline auto get_first(ptrdiff_t offset = 0)
        {
            return get_one().get<T>(offset);
        }
    };

    class HRY_API module_pattern : public pattern
    {
    public:
        template<size_t Len>
        module_pattern(void* module, const char (&pattern)[Len]) : pattern(module)
        {
            Initialize(pattern, Len);
        }
    };

    class HRY_API range_pattern : public pattern
    {
    public:
        template<size_t Len>
        range_pattern(uintptr_t begin, uintptr_t end, const char (&pattern)[Len])
            : pattern(begin, end)
        {
            Initialize(pattern, Len);
        }
    };

    template<typename T = void, size_t Len>
    auto get_pattern(const char (&pattern_string)[Len], ptrdiff_t offset = 0)
    {
        return pattern(pattern_string).get_first<T>(offset);
    }

    template<typename T = void>
    auto get_pattern(std::string_view pattern_view, ptrdiff_t offset = 0)
    {
        return pattern(pattern_view).get_first<T>(offset);
    }
} // namespace hry

#pragma warning(pop)