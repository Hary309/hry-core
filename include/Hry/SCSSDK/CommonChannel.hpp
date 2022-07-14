/**
 * This file is part of the hry-core project
 * @ Author: Piotr Krupa <piotrkrupa06@gmail.com>
 * @ License: MIT License
 * @ Documentation: SCS Software
 */

#pragma once

#include <cstdint>

namespace hry::scs
{
/**
  * @brief Common game channels
  *
  */
struct CommonChannel
{
  /**
    * @brief Scale applied to distance and time to compensate
    * for the scale of the map (e.g. 1s of real time corresponds to local_scale
    * seconds of simulated game time).
    *
    * Games which use real 1:1 maps will not provide this
    * channel.
    */
  float localScale;

  /**
    * @brief Absolute in-game time.
    *
    * Represented in number of in-game minutes since beginning (i.e. 00:00)
    * of the first in-game day.
    *
    */
  uint32_t gameTime;

  /**
    * @brief Time until next rest stop.
    *
    * When the fatique simulation is disabled, the behavior of this channel
    * is implementation dependent. The game might provide the value which would
    * apply if it was enabled or provide no value at all.
    *
    * Represented in in-game minutes.
    */
  int32_t restStop;
};
} // namespace hry::scs
