#pragma once
#include "Levels.hpp"
#include <chrono>
#include <optional>

struct GameFinished
{
    // TODO: Put level stats here!
};

class Updater
{
public:
    Updater(LevelData& level_) : level(level_) {}

    std::optional<GameFinished> run(std::chrono::duration<float> dt);

private:
    LevelData& level;
};

