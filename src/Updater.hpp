#pragma once
#include "GameState.hpp"
#include <chrono>

class Updater
{
public:
    Updater(GameState& state) : state_(state) {}

    void run(std::chrono::duration<float> dt);

private:
    GameState& state_;
};