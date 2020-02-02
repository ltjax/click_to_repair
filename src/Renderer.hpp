#pragma once
#include "GameState.hpp"
#include <chrono>
#include <onut/SpriteBatch.h>

class Renderer
{
public:
    Renderer(GameState const& state) : state_(state) {}

    void init();
    void run();
private:
    GameState const& state_;
};