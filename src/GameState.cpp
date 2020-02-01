#include "GameState.hpp"
#include <onut/Input.h>
#include <onut/onut.h>
#include "WinScreen.hpp"
#include "InGameScreen.hpp"

std::unique_ptr<Screen> MainMenuScreen::update(std::chrono::duration<float> dt)
{
    if (OInputJustPressed(OKeyEscape))
    {
        OQuit();
        return nullptr;
    }

    if (OInputPressed(OMouse1))
        return std::make_unique<InGameScreen>();

    return nullptr;
}

void MainMenuScreen::render()
{
    // TODO: render something...
}

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

