#include "GameState.hpp"
#include "MainMenuScreen.hpp"
#include "WinScreen.hpp"
#include <onut/Input.h>
#include <onut/onut.h>

//void setup_level(IngameScreen& state, int level);
//void advance_level(IngameScreen& state);
//void win_level(IngameScreen& state);
//
//
//void advance_level(IngameScreen& state)
//{
//    auto next_level_index = state.level_index + 1;
//    if (next_level_index >= MAX_LEVEL) {
//        state.menu_state = MenuState{};
//    }
//    else
//        setup_level(state, next_level);
//}

//void win_level(IngameScreen& state)
//{
//    // TODO: first show win screen
//    advance_level(state);
//}

IngameScreen::IngameScreen()
    : level(create_level(0)), updater(level), renderer(level)
{
}

std::unique_ptr<Screen> IngameScreen::update(std::chrono::duration<float> dt)
{
    auto finished = updater.run(dt);

    if (finished)
    {
        return std::make_unique<WinScreen>();
    }

    if (OInputJustPressed(OKeyEscape))
        return std::make_unique<MainMenuScreen>();
//
//#ifndef NDEBUG
//    if (OInputJustPressed(OKeyRight))
//        advance_level(*this);
//#endif
    return nullptr;
}

void IngameScreen::render()
{
    renderer.run();
}
