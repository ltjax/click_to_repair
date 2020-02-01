#pragma once
#include <memory>
#include "Levels.hpp"
#include "Updater.hpp"
#include "Renderer.hpp"
#include "Screen.hpp"

struct MainMenuScreen : Screen
{
    std::unique_ptr<Screen> update(std::chrono::duration<float> dt) override;
    void render() override;
};

struct IngameScreen : Screen
{
    IngameScreen();
    std::unique_ptr<Screen> update(std::chrono::duration<float> dt) override;
    void render() override;

    int level_index = 0;
    LevelData level;
    Updater updater;
    Renderer renderer;
};

struct GameState
{
    std::unique_ptr<Screen> menu_state; //std::make_unique<MainMenuScreen>();
};

void enter_main_menu(GameState&);
