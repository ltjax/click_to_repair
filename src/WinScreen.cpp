#include "WinScreen.hpp"
#include <onut/Input.h>
#include <onut/onut.h>
#include <onut/Renderer.h>
#include <onut/SpriteBatch.h>
#include "Constants.hpp"
#include "MainMenuScreen.hpp"

WinScreen::WinScreen()
{
    checkmark_ = OGetTexture("level_complete.png");

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

    if (OInputJustPressed(OKeyEscape))
    {
        OQuit();
        return nullptr;
    }

    if (OInputJustPressed(OMouse1))
    {
        return std::make_unique<MainMenuScreen>();
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
