#include "WinScreen.hpp"
#include <onut/Input.h>
#include <onut/onut.h>
#include <onut/Renderer.h>
#include <onut/SpriteBatch.h>
#include "Constants.hpp"
#include "GameState.hpp"

WinScreen::WinScreen()
{
    checkmark_ = OGetTexture("level_complete.png");
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
    oRenderer->clear(Constants::BackgroundColor());
    oSpriteBatch->begin();
    oSpriteBatch->drawSprite(checkmark_, OScreenCenterf);
    oSpriteBatch->end();
}
