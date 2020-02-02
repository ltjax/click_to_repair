#include "WinScreen.hpp"
#include <onut/Input.h>
#include <onut/onut.h>
#include <onut/Renderer.h>
#include <onut/SpriteBatch.h>
#include "Constants.hpp"
#include "MainMenuScreen.hpp"
#include "InGameScreen.hpp"
#include "CreditsScreen.hpp"

WinScreen::WinScreen(std::shared_ptr<SharedState> sharedState, int finished_level)
  : sharedState_(sharedState), finished_level_(finished_level)
{
    checkmark_ = OGetTexture("level_complete.png");
    OPlaySound("level_complete.wav");

    anim_.play(
        2.f, // From
        1, // To
        2.f, // Duration in Seconds
        OTweenBounceOut, // Tween
        ODontLoop // Loop
    );
}

Screen::Factory WinScreen::update(std::chrono::duration<float> dt)
{
    const auto next_level = finished_level_ + 1;
    if (OInputJustPressed(OKeyEscape))
    {
        return [state = sharedState_] {return std::make_unique<MainMenuScreen>(state);};
    }

    if (OInputJustPressed(OMouse1))
    {
        if (next_level == Constants::MAX_LEVELS())
            return [state = sharedState_] {return std::make_unique<CreditsScreen>(state); };
        return [state = sharedState_, next_level] {return std::make_unique<InGameScreen>(state, next_level);};
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
