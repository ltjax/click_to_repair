#pragma once
#include <memory>
#include "Screen.hpp"

struct MainMenuScreen : Screen
{
    std::unique_ptr<Screen> update(std::chrono::duration<float> dt) override;
    void render() override;
};

struct GameState
{
    std::unique_ptr<Screen> menu_state; //std::make_unique<MainMenuScreen>();
};

void enter_main_menu(GameState&);
