#include "MainMenuScreen.hpp"
#include "InGameScreen.hpp"
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
    return std::make_unique<InGameScreen>(0);

  return nullptr;
}

void MainMenuScreen::render()
{
  // TODO: render something...
}