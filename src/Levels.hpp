#pragma once

#include <entt/entt.hpp>

enum class Quality
{
    Worst,
    Medium,
    Good
};

struct LevelData
{
    entt::registry entities;
    float repairium = 0.f;
    Quality quality = Quality::Good;
    bool is_repairing = false;
    bool started_repairing = false;
    float repair_time = 0.f;
    bool won_level = false;
};

LevelData create_level(int index);
