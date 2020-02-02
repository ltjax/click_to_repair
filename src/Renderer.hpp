#pragma once
#include "Levels.hpp"
#include <chrono>
#include <onut/SpriteBatch.h>

class Renderer
{
public:
    Renderer(LevelData const& level_) : level(level_) {}

    void run();
private:
    LevelData const& level;
};