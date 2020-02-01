#include "InGameScreen.hpp"
#include "MainMenuScreen.hpp"
#include "WinScreen.hpp"
#include <onut/Input.h>

InGameScreen::InGameScreen(int levelNumber)
    : level(create_level(levelNumber)), updater(level), renderer(level)
{
}

std::unique_ptr<Screen> InGameScreen::update(std::chrono::duration<float> dt)
{
    auto finished = updater.run(dt);

    if (finished)
    {
        return std::make_unique<WinScreen>();
    }

    if (OInputJustPressed(OKeyEscape))
        return std::make_unique<MainMenuScreen>();

    return nullptr;
}

void InGameScreen::render()
{
    renderer.run();
}