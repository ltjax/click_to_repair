#include "CreditsScreen.hpp"
#include <onut/Renderer.h>
#include <onut/Font.h>
#include <onut/SpriteBatch.h>
#include <onut/Input.h>
#include <onut/Sound.h>
#include "Constants.hpp"
#include "MainMenuScreen.hpp"
#include "utils.hpp"

CreditsScreen::CreditsScreen(std::shared_ptr<SharedState> sharedState)
    : sharedState_(sharedState)
{
    OPlaySound("level_complete.wav");

    anim_.play(
        -20.f, // From
        20.f, // To
        3.f, // Duration in Seconds
        OTweenEaseBoth, // Tween
        OPingPongLoop // Loop
    );

    zoom_.play(
        2.f, // From
        1, // To
        2.f, // Duration in Seconds
        OTweenBounceOut, // Tween
        ODontLoop // Loop
    );
    checkmark_ = OGetTexture("level_complete.png");
}

Screen::Factory CreditsScreen::update(std::chrono::duration<float> dt)
{
    if (OInputJustPressed(OKeyEscape) || (OInputJustPressed(OMouse1) && !get_fullscreen_rect().Contains(oInput->mousePosf)))
    {
        return [state = sharedState_] {return std::make_unique<MainMenuScreen>(state); };
    }
    return {};
}

void CreditsScreen::render()
{
    auto fontBig = OGetFont("mainBig.fnt");
    auto font = OGetFont("main.fnt");
    oRenderer->clear(Constants::BackgroundColor());

    oSpriteBatch->begin();
    oSpriteBatch->drawSprite(checkmark_, OScreenCenterf, Color::White, anim_.get(), 0.33f * zoom_.get());
    oSpriteBatch->end();

    std::string Lines[]{
        "A game jammed at #GGJ2020 by:",
        " Marius Elvert",
        " Alexander Kasper",
        " Thomas Kranz",
        " Sven Moellers",
        " Fabian Sperber",
        "",
        "",
        " Additional Music: Dr. Matze"
    };

    Vector2 position(10, get_fullscreen_rect().Bottom().y + 20);

    for (auto const& Each : Lines)
    {
        font->draw(Each, position);
        position.y += 30.f;
    }

    auto screen = OScreenf;
    fontBig->draw("Thank you for playing!", Vector2(10.f, screen.y - 70.f));
}
