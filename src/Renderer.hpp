#pragma once
#include "Levels.hpp"
#include <chrono>
#include <onut/SpriteBatch.h>
#include <onut/Anim.h>


class Renderer
{
public:
    Renderer(LevelData const& level_);

    void run();
private:
    LevelData const& level;
    OAnimFloat anim_critical_machines_;
};