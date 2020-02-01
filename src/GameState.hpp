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
    float repairium = 0;
    Quality quality = Quality::Good;
    entt::registry entities;
};