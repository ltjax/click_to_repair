#pragma once
#include <memory>
#include "Screen.hpp"

struct Progress
{
  int next_available_level = 0;

  void load();
  void save();
};

struct SharedState
{
  Progress progress;
};

struct GameState
{
  std::unique_ptr<Screen> menu_state;
  std::shared_ptr<SharedState> shared_state;
};
