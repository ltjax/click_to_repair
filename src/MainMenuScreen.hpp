#pragma once

#include "Screen.hpp"
#include "GameState.hpp"
#include <optional>
#include <onut/Anim.h>

struct MainMenuScreen : Screen
{
  MainMenuScreen(Progress& progress_);
  std::unique_ptr<Screen> update(std::chrono::duration<float> dt) override;
  void render() override;

  Progress& progress;
  std::optional<int> level_in_focus;
  OAnimFloat anim_current_level_;
  OAnimFloat anim_main_logo_;
};
