#pragma once

#include <variant>

#include <scssdk_telemetry.h>

#include "Hry/Math/SCSTypes.hpp"
#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

namespace scs
{
    struct ValueType
    {
        using Variant_t = std::variant<
            std::monostate,
            bool,
            int32_t,
            uint32_t,
            uint64_t,
            float,
            double,
            Vec3<float>,
            Vec3<double>,
            Euler,
            PlacementF,
            PlacementD,
            const char*,
            int64_t>;

        Variant_t value;

        static ValueType Get(const scs_value_t& value);
    };
} // namespace scs

HRY_NS_END

template<>
struct fmt::formatter<hry::scs::ValueType::Variant_t>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(hry::scs::ValueType::Variant_t const& v, FormatContext& ctx)
    {
        auto iter = fmt::format_to(ctx.out(), "");

        std::visit(
            [&](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;

                if constexpr (!std::is_same_v<T, std::monostate>)
                {
                    iter = fmt::format_to(ctx.out(), "{}", arg);
                }
            },
            v);

        return iter;
    }
};
