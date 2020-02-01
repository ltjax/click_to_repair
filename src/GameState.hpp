#pragma once
#include <memory>
#include "Screen.hpp"

struct Progress
{
  int next_available_level = 0;

  void load();
  void save();
};

struct GameState
{
  std::unique_ptr<Screen> menu_state;
  Progress progress;
};
