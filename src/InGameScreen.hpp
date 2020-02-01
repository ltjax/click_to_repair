#pragma once
#include "Screen.hpp"
#include "Levels.hpp"
#include "Updater.hpp"
#include "Renderer.hpp"

struct InGameScreen : Screen
{
    InGameScreen();
    std::unique_ptr<Screen> update(std::chrono::duration<float> dt) override;
    void render() override;

    int level_index = 0;
    LevelData level;
    Updater updater;
    Renderer renderer;
};
