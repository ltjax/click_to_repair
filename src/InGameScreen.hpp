#pragma once
#include "Screen.hpp"
#include "Levels.hpp"
#include "Updater.hpp"
#include "Renderer.hpp"
#include "GameState.hpp"

struct InGameScreen : Screen
{
    InGameScreen(std::shared_ptr<SharedState> sharedState, int levelNumber);
    ScreenFactory update(std::chrono::duration<float> dt) override;
    void render() override;

    std::shared_ptr<SharedState> sharedState;
    int levelNumber;
    LevelData level;
    Updater updater;
    Renderer renderer;

    ~InGameScreen();
};
