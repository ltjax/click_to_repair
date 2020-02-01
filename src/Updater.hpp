#pragma once
#include "Levels.hpp"
#include <chrono>

class Updater
{
public:
    Updater(LevelData& level_) : level(level_) {}

    void run(std::chrono::duration<float> dt);

private:
    LevelData& level;
};

