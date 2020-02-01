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

// Global game state
GameState gameState;
Updater updater(gameState);
Renderer renderer(gameState);

entt::entity createGear(entt::registry& registry, Vector2 position)
{
    auto gear = registry.create();
    registry.assign<Machine>(gear, position, 128.f);
    registry.assign<Gear>(gear);
    registry.assign<Durability>(gear, 1.f);
    return gear;
}

void initSettings()
{
    oSettings->setGameName("Click to repair!");
}

void init()
{
    auto& registry = gameState.entities;

    oContentManager->addSearchPath("../../../../assets");

    createGear(registry, OScreenCenterf / 2.0f);
    createGear(registry, OScreenCenterf + OScreenCenterf / 2.0f);
}

void update()
{
    auto dt = std::chrono::duration<float>{ ODT };
    updater.run(dt);
}

void render()
{
    renderer.run();
}

void postRender()
{
}

void renderUI()
{
    auto pFont = OGetFont("main.fnt");
    pFont->draw("Quality " + std::to_string((int)(gameState.quality * 100.f)) + "%", { 10, 300 }, OLeft);
    pFont->draw("Repairum " + std::to_string(gameState.repairium), { 10, 320 }, OLeft);
}