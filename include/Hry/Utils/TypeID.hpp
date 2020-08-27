#include "Hash.hpp"

HRY_NS_BEGIN

template<typename T>
constexpr Hash64_t TypeID() noexcept
{
    return FNV1a_64(__PRETTY_FUNCTION__);
}

HRY_NS_END