#pragma once
#include "Screen.hpp"
#include "Levels.hpp"
#include "Updater.hpp"
#include "Renderer.hpp"
#include "GameState.hpp"

struct InGameScreen : Screen
{
    InGameScreen(Progress& progress, int levelNumber);
    std::unique_ptr<Screen> update(std::chrono::duration<float> dt) override;
    void render() override;

    Progress& progress;
    int levelNumber;
    LevelData level;
    Updater updater;
    Renderer renderer;

    ~InGameScreen();
};
