// Oak Nut include
#include <onut/Anim.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/Sound.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>
#include <onut/Input.h>
#include <onut/Window.h>
#include <onut/onut.h>
#include <onut/Component.h>
#include <onut/ContentManager.h>
#include <filesystem>
#include <algorithm>
#include <entt/entt.hpp>
#include <onut/Font.h>
#include "utils.hpp"
#include "GameState.hpp"
#include "Updater.hpp"
#include "Renderer.hpp"
#include "Components.hpp"
#include "MainMenuScreen.hpp"
#include "InGameScreen.hpp"

// Global game state
GameState gameState;

void initSettings()
{
    oSettings->setGameName("Click to repair!");
    oSettings->setResolution({1280, 1024});
}

void init()
{
    std::srand(std::time(nullptr));
    oContentManager->addSearchPath("../../../../assets");
    gameState.shared_state = std::make_shared<SharedState>();
    gameState.menu_state = std::make_unique<MainMenuScreen>(gameState.shared_state);
    gameState.shared_state->progress.load();
}

void update()
{
    auto dt = std::chrono::duration<float>{ ODT };
    auto next_state_factory = gameState.menu_state->update(dt);
    if (next_state_factory)
    {
        gameState.menu_state.reset();
        gameState.menu_state = next_state_factory();
    }
}

void render()
{
    gameState.menu_state->render();
}

void postRender()
{
}

void renderUI()
{
}