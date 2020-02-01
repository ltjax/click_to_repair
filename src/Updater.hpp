#pragma once
#include "GameState.hpp"
#include <chrono>

class Updater
{
public:
    Updater(GameState& state_) : state(state_) {}

    void run(std::chrono::duration<float> dt);

private:
    GameState& state;
};