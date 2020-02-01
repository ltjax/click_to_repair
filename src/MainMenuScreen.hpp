#pragma once

#include "Screen.hpp"
#include "GameState.hpp"
#include <optional>
#include <onut/Anim.h>

struct MainMenuScreen : Screen
{
  MainMenuScreen(std::shared_ptr<SharedState> sharedState);
  ScreenFactory update(std::chrono::duration<float> dt) override;

  void render() override;

  std::shared_ptr<SharedState> sharedState;
  std::optional<int> level_in_focus;

  OAnimFloat anim_current_level_;
  OAnimFloat anim_main_logo_;
  
  ~MainMenuScreen()
  {
      backgroundMusic->stop();
  }
};
