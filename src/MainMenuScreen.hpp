#pragma once

#include "Screen.hpp"

struct MainMenuScreen : Screen
{
  std::unique_ptr<Screen> update(std::chrono::duration<float> dt) override;
  void render() override;
};
