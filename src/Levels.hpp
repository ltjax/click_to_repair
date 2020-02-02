#pragma once

#include <entt/entt.hpp>
#include <onut/Matrix.h>
#include <random>
#include "Components.hpp"


struct LevelData
{
    entt::registry entities;
    float last_repairium = 0.f;
    float repairium = 0.f;
    QualityStatus quality;
    bool is_repairing = false;
    bool started_repairing = false;
    float repair_time = 0.f;
    bool won_level = false;
    Matrix camera;
    std::mt19937 rng;
    float reparium_multiplier = 1.f;
    float animation_factor = 1.f;
};

LevelData create_level(int index);
