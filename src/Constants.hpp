#pragma once
#include <onut/Color.h>
#include <chrono>

namespace Constants
{
    Color BackgroundColor();

    inline constexpr auto REPAIR_RESOURCE_DRAIN_PER_SECOND = 0.05f;
    inline constexpr auto REPAIR_DURATION_INCREASE_PER_SECOND_WITH_RESOURCE = 0.5f;
    inline constexpr auto REPAIR_DURATION_INCREASE_PER_SECOND_WHEN_EMPTY = 0.3f;
    inline constexpr auto REPAIR_UP_FRONT_COST = 0.05f;

    inline constexpr auto GEAR_WEAR_PER_SECOND = 0.070f;
    inline constexpr auto GEAR_RATIO = -17.f/9.f;
    inline constexpr auto ENGINE_WEAR_PER_SECOND_SHAKING = 0.3f;
    inline constexpr auto ENGINE_WEAR_PER_SECOND = 0.05f;
    inline constexpr auto ENGINE_SHAKE_DURATION = std::chrono::milliseconds(1200);

    inline constexpr auto FLUX_CAPACITOR_OVERLOAD_TIME = std::chrono::milliseconds(2500);
    inline constexpr auto FLUX_CAPACITOR_WEAR_PER_SECOND = 0.05f;

    int MAX_LEVELS(); // NOTE: Implementation of this is in Levels.cpp!
}
