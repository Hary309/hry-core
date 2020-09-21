#pragma once

#include <array>

#include <common/scssdk_telemetry_common_configs.h>

#include "Hry/SCSSDK/TrailerChannel.hpp"

#include "Events/EventProxyBase.hpp"
#include "Events/InternalEventHandler.hpp"

#include "ChannelAggregatorBase.hpp"

HRY_NS_BEGIN

class TrailerChannelAggregator : public ChannelAggregatorBase
{
public:
    using Trailers_t =
        std::array<std::unique_ptr<scs::TrailerChannel>, SCS_TELEMETRY_trailers_count>;

private:
    Trailers_t& _trailers;

    std::array<uint32_t, SCS_TELEMETRY_trailers_count> _wheelCount{};

public:
    TrailerChannelAggregator(
        Trailers_t& trailerChannel,
        scs_telemetry_init_params_v100_t* scsTelemetry,
        InternalEventHandler& eventHandler);

private:
    template<typename ValueType>
    void registerChannel(const char* id, int trailerIndex, ValueType& member)
    {
        ChannelAggregatorBase::registerChannel(GetIndexedId(id, trailerIndex).c_str(), member);
    }

    template<typename ValueType>
    void registerIndexedChannel(
        const char* id,
        int trailerIndex,
        uint32_t beginIndex,
        uint32_t endIndex,
        std::vector<ValueType>& member)
    {
        ChannelAggregatorBase::registerIndexedChannel(
            GetIndexedId(id, trailerIndex).c_str(), beginIndex, endIndex, member);
    }

    template<typename ValueType>
    void unregisterIndexedChannel(
        const char* id,
        int trailerIndex,
        uint32_t startIndex,
        uint32_t endIndex,
        std::vector<ValueType>& member)
    {
        ChannelAggregatorBase::unregisterIndexedChannel(
            GetIndexedId(id, trailerIndex).c_str(), startIndex, endIndex, member);
    }

    void registerTrailer(int index);

    void onTrailerConfig(const std::optional<scs::Trailer>&&);

    void registerWheels(int trailerIndex, int startIndex, int endIndex);
    void unregisterWheels(int trailerIndex, int startIndex, int endIndex);

    static std::string GetIndexedId(const char* id, int index);
};

HRY_NS_END
