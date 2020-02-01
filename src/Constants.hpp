#pragma once
#include <onut/Color.h>

namespace Constants
{
    Color BackgroundColor();

    inline constexpr auto REPAIR_RESOURCE_DRAIN_PER_SECOND = 0.02f;
    inline constexpr auto REPAIR_DURATION_INCREASE_PER_SECOND_WITH_RESOURCE = 0.25f;
    inline constexpr auto REPAIR_DURATION_INCREASE_PER_SECOND_WHEN_EMPTY = 0.03f;
    inline constexpr auto REPAIR_UP_FRONT_COST = 0.1f;

    inline constexpr auto WEAR_PER_SECOND = 0.10f;
}
