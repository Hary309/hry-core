/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 * @ Documentation: SCS Software
 */

#pragma once

#include <cstdint>
#include <string>

namespace hry::scs
{
/* -- Common types -- */

/** @brief Fine offence type */
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
    /** @brief The id of the transportation */
    std::string id;
    /** @brief The name of the transportation */
    std::string name;
};

/* -- Main structures -- */

/**
 * @brief Event called when job is cancelled
 */
struct JobCancelled
{
    /** @brief The penalty for cancelling the job in native game currency. (Can be 0) */
    int64_t cancelPenalty;
};

/**
 * @brief Event called when job is delivered
 */
struct JobDelivered
{
    /** @brief The job revenue in native game currency */
    int64_t revenue;

    /** @brief How much XP player received for the job */
    int32_t earnedXP;

    /** @brief Total cargo damage. (Range <0.0, 1.0>) */
    float cargoDamage;

    /** @brief The real distance in km on the job */
    float distanceKM;

    /** @brief Total time spend on the job in game minutes */
    int64_t deliveryTime;

    /** @brief Was auto parking used on this job? */
    bool autoParkUsed;

    /** @brief Was auto loading used on this job? (always @c true for non cargo market jobs) */
    bool autoLoadUsed;
};

struct PlayerFined
{
    /** @brief Fine offence type */
    FineOffenceType fineOffence;

    /** @brief Fine offence amount in native game currency */
    int64_t fineAmount;
};

struct PlayerTollgatePaid
{
    /** @brief How much player was charged for this action (in native game currency) */
    int64_t payAmount;
};

struct PlayerUseFerry
{
    /** @brief How much player was charged for this action (in native game currency) */
    int64_t payAmount;

    /** @brief The transportation source */
    Place source;

    /** @brief The transportation target */
    Place target;
};

struct PlayerUseTrain
{
    /** @brief How much player was charged for this action (in native game currency) */
    int64_t payAmount;

    /** @brief The transportation source */
    Place source;

    /** @brief The transportation target */
    Place target;
};
}
