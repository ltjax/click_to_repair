#pragma once
#include <entt/entt.hpp>

enum class Quality
{
    Worst,
    Medium,
    Good
};

struct GameState
{
    float repairium = 0.f;
    Quality quality = Quality::Good;
    entt::registry entities;
    bool is_repairing = false;
    float repair_time = 0.f;
};