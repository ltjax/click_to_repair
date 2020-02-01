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
#include "Components.hpp"

GameState gameState;
Updater updater(gameState);

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

void renderGears(entt::registry& registry)
{
    auto texture = OGetTexture("gear.png");
    auto view = registry.view<Machine, Gear>();
    for (auto entity : view)
    {
        auto const& machine = view.get<Machine>(entity);
        auto const& gear = view.get<Gear>(entity);

        auto transform = gear.rotation * Matrix::CreateTranslation(machine.position.x, machine.position.y, 0.f);
        auto textureSize = texture->getSizef();

        oRenderer->renderStates.blendMode = OBlendAlpha;
        oSpriteBatch->drawSprite(texture, transform, Vector2(machine.size / textureSize.x, machine.size / textureSize.y));
    }
}

void renderBar(OSpriteBatchRef spriteBatch, Rect rectangle, float fullness)
{
    auto contentRect = shrinkRect(rectangle, Vector2{ 1 });
    contentRect.z *= fullness;
    oSpriteBatch->drawRect(nullptr, rectangle, Color::White);

    auto color = OColorRGB(88, 88, 88);
    if (fullness < 0.1f)
    {
        color = OColorRGB(220, 20, 20);
    }
    else if (fullness < 0.5f)
    {
        color = OColorRGB(220, 220, 20);
    }
    oSpriteBatch->drawRect(nullptr, contentRect, color);
}

void renderDurabilityBar(entt::registry& registry)
{
    auto view = registry.view<Machine, Durability>();

    for (auto entity : view)
    {
        auto const& machine = view.get<Machine>(entity);
        auto durability = view.get<Durability>(entity).durability;

        auto p = machine.position;
        auto size = machine.size;
        auto halfSize = size * 0.5f;
        auto barHeight = 10.f;
        auto backgroundRect = Rect{ -halfSize + p.x, -halfSize + p.y - barHeight, size, barHeight };
        renderBar(oSpriteBatch, backgroundRect, durability);
    }
}

void renderRepairiumBar(GameState const& state)
{
    auto barHeight = 32.f;
    auto screenSize = OScreenf;
    auto rect = Rect{0.f, screenSize.y - barHeight, screenSize.x, barHeight};
    rect = shrinkRect(rect, Vector2{8.f});
    renderBar(oSpriteBatch, rect, state.repairium);
}


void update()
{
    auto dt = std::chrono::duration<float>{ ODT };
    updater.run(dt);
    if (OInputJustPressed(OKeyEscape))
        OQuit();
}

void render()
{
    oRenderer->clear(OColorHex(556677));
    oSpriteBatch->begin();

    auto& registry = gameState.entities;
    renderGears(registry);
    renderDurabilityBar(registry);
    renderRepairiumBar(gameState);

    oSpriteBatch->end();
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