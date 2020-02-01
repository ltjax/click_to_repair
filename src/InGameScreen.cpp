#include "InGameScreen.hpp"
#include "MainMenuScreen.hpp"
#include "WinScreen.hpp"
#include <onut/Input.h>

InGameScreen::InGameScreen(Progress& progress_, int levelNumber_)
    : progress(progress_), levelNumber(levelNumber_), level(create_level(levelNumber)), updater(level), renderer(level)
{
    backgroundMusic = OGetMusic("background_music.ogg");
    backgroundMusic->play();
}

std::unique_ptr<Screen> InGameScreen::update(std::chrono::duration<float> dt)
{
    auto finished = updater.run(dt);

    if (finished)
    {
        return std::make_unique<WinScreen>(progress, levelNumber);
    }

    if (OInputJustPressed(OKeyEscape))
        return std::make_unique<MainMenuScreen>(progress);

    return nullptr;
}

void InGameScreen::render()
{
    renderer.run();
}

InGameScreen::~InGameScreen()
{
    backgroundMusic->stop();
}