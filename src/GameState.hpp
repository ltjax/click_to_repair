#pragma once
#include <memory>
#include "Screen.hpp"

struct Progress
{
  int next_available_level = 0;
};

struct GameState
{
  std::unique_ptr<Screen> menu_state;
  Progress progress;
};

void enter_main_menu(GameState&);
