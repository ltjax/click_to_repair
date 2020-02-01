#include "MainMenuScreen.hpp"
#include "GameState.hpp"
#include <onut/Input.h>
#include <onut/onut.h>

std::unique_ptr<Screen> MainMenuScreen::update(std::chrono::duration<float> dt)
{
  if (OInputJustPressed(OKeyEscape))
  {
    OQuit();
    return nullptr;
  }

  if (OInputPressed(OMouse1))
    return std::make_unique<IngameScreen>();

  return nullptr;
}

void MainMenuScreen::render()
{
  // TODO: render something...
}