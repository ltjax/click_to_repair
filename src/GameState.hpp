#pragma once
#include <entt/entt.hpp>

struct GameState
{
    float repairium = 0;
    float quality = 1.f;
    entt::registry entities;
};