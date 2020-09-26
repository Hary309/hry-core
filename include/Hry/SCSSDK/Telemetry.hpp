#pragma once

#include <array>
#include <memory>

#include "Hry/Namespace.hpp"
#include "Hry/SCSSDK/CommonChannel.hpp"
#include "Hry/SCSSDK/ConfigurationData.hpp"
#include "Hry/SCSSDK/JobChannel.hpp"
#include "Hry/SCSSDK/TrailerChannel.hpp"
#include "Hry/SCSSDK/TruckChannel.hpp"

HRY_NS_BEGIN

class ChannelAggregator;

struct Telemetry final
{
    friend ChannelAggregator;

public:
    static constexpr int MaxTrailerCount = 10;

private:
    // use pointers to avoid wrong offsets when more fields are added
    std::unique_ptr<scs::TruckChannel> _truck;
    std::array<std::unique_ptr<scs::TrailerChannel>, Telemetry::MaxTrailerCount> _trailers;
    std::unique_ptr<scs::JobChannel> _job;
    std::unique_ptr<scs::CommonChannel> _common;

    scs::Substances _substances;

private:
    Telemetry()
        : _truck(std::make_unique<scs::TruckChannel>()), _job(std::make_unique<scs::JobChannel>()),
          _common(std::make_unique<scs::CommonChannel>())
    {
        for (int i = 0; i < Telemetry::MaxTrailerCount; ++i)
        {
            _trailers[i] = std::make_unique<scs::TrailerChannel>();
        }
    }
    ~Telemetry() = default;

public:
    Telemetry(Telemetry&&) = delete;
    Telemetry(const Telemetry&) = delete;
    Telemetry& operator=(Telemetry&&) = delete;
    Telemetry& operator=(const Telemetry&) = delete;

public:
    const scs::TruckChannel* getTruck() const { return _truck.get(); }
    const scs::TrailerChannel* getTrailer(int index) const
    {
        if (index >= 0 && index <= Telemetry::MaxTrailerCount)
        {
            return _trailers[index].get();
        }

        return nullptr;
    }
    const scs::JobChannel* getJob() const { return _job.get(); }
    const scs::CommonChannel* getCommon() const { return _common.get(); }

    const scs::Substances* getSubstances() const { return &_substances; }
};

HRY_NS_END
