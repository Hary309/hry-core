#pragma once

#include <cstdint>
#include <string>

#include "Hry/Namespace.hpp"

HRY_NS_BEGIN

namespace scs
{
    /* -- Common types -- */
    enum class FineOffenceType
    {
        Crash,
        AvoidSleeping,
        WrongWay,
        SpeedingCamera,
        NoLights,
        RedSignal,
        Speeding,
        AvoidWeighting,
        IllegalTrailer,
        AvoidInspection,
        IllegalBorderCrossing,
        HardShoulderViolation,
        DamagedVehicleUsage,
        Generic
    };

    struct Place
    {
        std::string id;
        std::string name;
    };

    /* -- Main structures -- */
    struct JobCancelled
    {
        int64_t cancelPenalty;
    };

    struct JobDelivered
    {
        int64_t revenue;
        int32_t earnedXP;
        float cargoDamage;
        float distanceKM;
        int64_t deliveryTime;
        bool autoParkUsed;
        bool autoLoadUsed;
    };

    struct PlayerFined
    {
        FineOffenceType fineOffence;
        int64_t fineAmount;
    };

    struct PlayerTollgatePaid
    {
        int64_t payAmount;
    };

    struct PlayerUseFerry
    {
        int64_t payAmount;
        std::string sourceName;
        Place source;
        Place target;
    };

    struct PlayerUseTrain
    {
        int64_t payAmount;
        std::string sourceName;
        Place source;
        Place target;
    };

} // namespace scs

HRY_NS_END
