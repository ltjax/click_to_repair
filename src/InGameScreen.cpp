#include "InGameScreen.hpp"
#include "MainMenuScreen.hpp"
#include "WinScreen.hpp"
#include <onut/Input.h>

InGameScreen::InGameScreen(std::shared_ptr<SharedState> sharedState_, int levelNumber_)
    : sharedState(sharedState_), levelNumber(levelNumber_), level(create_level(levelNumber)), updater(level), renderer(level)
{
    backgroundMusic = OGetMusic("background_music.ogg");
    backgroundMusic->play();
}

Screen::ScreenFactory InGameScreen::update(std::chrono::duration<float> dt)
{
    auto finished = updater.run(dt);

    if (finished)
    {
        return [sharedState = sharedState, levelNumber = levelNumber]() {return std::make_unique<WinScreen>(sharedState, levelNumber);};
    }

    if (OInputJustPressed(OKeyEscape))
        return [sharedState = sharedState]() {return std::make_unique<MainMenuScreen>(sharedState); };

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