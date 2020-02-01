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
#include "InGameScreen.hpp"

// Global game state
GameState gameState;

void initSettings()
{
    oSettings->setGameName("Click to repair!");
}

void init()
{
    oContentManager->addSearchPath("../../../../assets");
    gameState.menu_state = std::make_unique<InGameScreen>(1);
}

void update()
{
    auto dt = std::chrono::duration<float>{ ODT };
    auto next_state = gameState.menu_state->update(dt);
    if (next_state)
    {
        gameState.menu_state = std::move(next_state);
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