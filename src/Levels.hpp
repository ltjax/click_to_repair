#pragma once

#include <entt/entt.hpp>
#include <onut/Matrix.h>
#include <random>
#include "Components.hpp"


struct LevelData
{
    entt::registry entities;
    float repairium = 0.f;
    QualityStatus quality;
    bool is_repairing = false;
    bool started_repairing = false;
    float repair_time = 0.f;
    bool won_level = false;
    Matrix camera;
    std::mt19937 rng;
};

LevelData create_level(int index);
