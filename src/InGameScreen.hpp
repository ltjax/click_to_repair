#pragma once
#include "Screen.hpp"
#include "Levels.hpp"
#include "Updater.hpp"
#include "Renderer.hpp"

struct InGameScreen : Screen
{
    InGameScreen(int levelNumber);
    std::unique_ptr<Screen> update(std::chrono::duration<float> dt) override;
    void render() override;

    LevelData level;
    Updater updater;
    Renderer renderer;
};
