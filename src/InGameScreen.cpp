#include "InGameScreen.hpp"
#include "MainMenuScreen.hpp"
#include "WinScreen.hpp"
#include <onut/Input.h>
#include "Constants.hpp"
#include "CreditsScreen.hpp"
#include <onut/Renderer.h>
#include "utils.hpp"

namespace
{
  const auto button_size = 96;
  Rect get_rect_from_top_right(int num)
  {
    return { OScreenWf - button_size * (num + 1), 0, Vector2(button_size) };
  }
}

InGameScreen::InGameScreen(std::shared_ptr<SharedState> sharedState_, int levelNumber_)
    : sharedState(sharedState_), levelNumber(levelNumber_), level(create_level(levelNumber)), updater(level), renderer(level)
{
    auto file = sharedState_->music.getNext_music(sharedState_->rng);
    backgroundMusic = OGetMusic(file);
    backgroundMusic->play();
}

Screen::Factory InGameScreen::update(std::chrono::duration<float> dt)
{

    if (OInputJustPressed(OMouse1))
    {
        if (get_rect_from_top_right(0).Contains(oInput->mousePosf))
            return [sharedState = sharedState]() { return std::make_unique<MainMenuScreen>(sharedState); };
        if (get_rect_from_top_right(1).Contains(oInput->mousePosf))
            level = create_level(levelNumber); // restart level
    }
    auto finished = updater.run(dt);

    if (finished)
    {
        auto next_level = levelNumber + 1;
        auto& progress_ = sharedState->progress;
        progress_.next_available_level = std::max(progress_.next_available_level, next_level);
        progress_.save();
        if (next_level == Constants::MAX_LEVELS())
            return [state = sharedState] {return std::make_unique<CreditsScreen>(state); };
        return [sharedState = sharedState, levelNumber = levelNumber]() {return std::make_unique<WinScreen>(sharedState, levelNumber);};
    }

    if (OInputJustPressed(OKeyEscape))
        return [sharedState = sharedState]() {return std::make_unique<MainMenuScreen>(sharedState); };

    return nullptr;
}

void InGameScreen::render()
{
    renderer.run();

    oSpriteBatch->begin();
    oSpriteBatch->drawRect(OGetTexture("back.png"), get_rect_from_top_right(0).Grow(-button_size * 0.1f), get_color_focus(get_rect_from_top_right(0)));
    oSpriteBatch->drawRect(OGetTexture("reset.png"), get_rect_from_top_right(1).Grow(-button_size * 0.1f), get_color_focus(get_rect_from_top_right(1)));
    oSpriteBatch->end();
}

InGameScreen::~InGameScreen()
{
    backgroundMusic->stop();
}