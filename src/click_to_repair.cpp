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

struct Durability
{
    float durability = 1.f;
};

struct Machine
{
    Vector2 position;
    float size = 128.f;

    Rect getBoundingBox() const
    {
        auto halfSize = size * 0.5f;
        return Rect{ -halfSize + position.x, -halfSize + position.y, size, size };
    }

};

struct Gear
{
    Matrix rotation;
};

struct GameState
{
    float repairium = 0;
    float quality = 1.f;
};
GameState state;

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

entt::registry oRegistry;

void init()
{
    oContentManager->addSearchPath("../../../../assets");

    createGear(oRegistry, OScreenCenterf / 2.0f);
    createGear(oRegistry, OScreenCenterf + OScreenCenterf / 2.0f);
}

void updateGears(entt::registry& registry, std::chrono::duration<float> dt)
{
    auto view = registry.view<Gear>();
    for (auto entity : view)
    {
        auto& gear = view.get<Gear>(entity);
        gear.rotation = Matrix::CreateRotationZ(dt.count()) * gear.rotation;
    }
}

void updateDuration(entt::registry& registry, std::chrono::duration<float> dt)
{
    auto view = registry.view<Durability, Machine>();

    for (auto entity : view)
    {
        auto const& machine = view.get<Machine>(entity);
        auto& durability = view.get<Durability>(entity).durability;
        if (OInputPressed(OMouse1) && machine.getBoundingBox().Contains(oInput->mousePosf))
        {
            auto speed = state.repairium > 0.f ? 0.15f : 0.03f;
            durability = std::min(1.f, durability + dt.count() * speed);
            state.repairium = std::max(0.f, state.repairium - dt.count() * 0.2f);
        }
        else
        {
            durability = std::max(0.f, durability - dt.count() * 0.05f);
        }
    }
}

void updateQuality(entt::registry& registry, std::chrono::duration<float> dt)
{
    auto view = registry.view<Durability>();

    float quality = 0.f;
    for (auto entity : view)
    {
        auto& durability = view.get<Durability>(entity).durability;

        quality += durability;
    }
    state.quality = quality / view.size();
}

void updateRepairum(std::chrono::duration<float> dt)
{
    if (state.quality < 0.1)
    {
        // machine is stopped
        return;
    }

    if (state.quality >= 0.8)
    {
        // machine is in SUPER condition and run
        state.repairium += 0.1 * dt.count();
    }

    // normal condition
    state.repairium += 0.05 * state.quality * dt.count();
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
    updateGears(oRegistry, dt);
    updateDuration(oRegistry, dt);
    updateQuality(oRegistry, dt);
    updateRepairum(dt);

    if (OInputJustPressed(OKeyEscape))
        OQuit();

}

void render()
{
    oRenderer->clear(OColorHex(556677));
    oSpriteBatch->begin();
    renderGears(oRegistry);
    renderDurabilityBar(oRegistry);
    renderRepairiumBar(state);

    oSpriteBatch->end();
}

void postRender()
{
}

void renderUI()
{
    auto pFont = OGetFont("main.fnt");
    pFont->draw("Quality " + std::to_string((int)(state.quality * 100.f)) + "%", { 10, 300 }, OLeft);
    pFont->draw("Repairum " + std::to_string(state.repairium), { 10, 320 }, OLeft);

}