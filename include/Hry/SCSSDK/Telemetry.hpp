#pragma once

#include <memory>

#include "Hry/Namespace.hpp"
#include "Hry/SCSSDK/CommonChannel.hpp"
#include "Hry/SCSSDK/JobChannel.hpp"
#include "Hry/SCSSDK/TrailerChannel.hpp"
#include "Hry/SCSSDK/TruckChannel.hpp"

HRY_NS_BEGIN

class ChannelAggregator;

struct Telemetry final
{
    friend ChannelAggregator;

private:
    // use pointers to avoid wrong offsets when more fields are added
    std::unique_ptr<scs::TruckChannel> _truck;
    std::unique_ptr<scs::TrailerChannel> _trailer; // TODO: make it array or sth
    std::unique_ptr<scs::JobChannel> _job;
    std::unique_ptr<scs::CommonChannel> _common;

private:
    Telemetry()
        : _truck(std::make_unique<scs::TruckChannel>()),
          _trailer(std::make_unique<scs::TrailerChannel>()),
          _job(std::make_unique<scs::JobChannel>()), _common(std::make_unique<scs::CommonChannel>())

    {
    }
    ~Telemetry() = default;

public:
    Telemetry(Telemetry&&) = delete;
    Telemetry(const Telemetry&) = delete;
    Telemetry& operator=(Telemetry&&) = delete;
    Telemetry& operator=(const Telemetry&) = delete;

public:
    const scs::TruckChannel* getTruck() const { return _truck.get(); }
    const scs::TrailerChannel* getTrailer() const { return _trailer.get(); }
    const scs::JobChannel* getJob() const { return _job.get(); }
    const scs::CommonChannel* getCommon() const { return _common.get(); }
};

HRY_NS_END
