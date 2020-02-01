#include "WinScreen.hpp"
#include <onut/Input.h>
#include <onut/onut.h>
#include <onut/Renderer.h>
#include <onut/SpriteBatch.h>
#include "Constants.hpp"
#include "MainMenuScreen.hpp"
#include "InGameScreen.hpp"

WinScreen::WinScreen(Progress& progress, int finished_level)
  : progress_(progress), finished_level_(finished_level)
{
  progress_.next_available_level = std::max(progress_.next_available_level, finished_level_ + 1);
    checkmark_ = OGetTexture("level_complete.png");
    sound_ = OCreateSoundInstance("level_complete.wav");
    sound_->play();

    anim_.play(
        2.f, // From
        1, // To
        2.f, // Duration in Seconds
        OTweenBounceOut, // Tween
        ODontLoop // Loop
    );
}

std::unique_ptr<Screen> WinScreen::update(std::chrono::duration<float> dt)
{
    const auto next_level = finished_level_ + 1;
    if (OInputJustPressed(OKeyEscape))
    {
      return std::make_unique<MainMenuScreen>(progress_);
    }

    if (OInputJustPressed(OMouse1))
    {
      return std::make_unique<InGameScreen>(progress_, next_level);
      //return std::make_unique<MainMenuScreen>(progress_, next_level);
    }

    return {};
}

void WinScreen::render()
{
    auto size = OScreenf;
    auto scale = anim_.get();
    auto w = std::min(size.x, size.y) * scale * 0.5f;
    auto offset = -w * 0.5f;

    Rect rect{offset, offset, w, w};
    oRenderer->clear(Constants::BackgroundColor());
    oSpriteBatch->begin(Matrix::CreateTranslation(OScreenCenterf));
    oSpriteBatch->drawRect(checkmark_, rect);
    oSpriteBatch->end();
}
