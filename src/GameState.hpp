#pragma once
#include <memory>
#include "Screen.hpp"

struct GameState
{
    std::unique_ptr<Screen> menu_state; //std::make_unique<MainMenuScreen>();
};

void enter_main_menu(GameState&);
