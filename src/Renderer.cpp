#include "Renderer.hpp"
#include "utils.hpp"
#include <onut/Texture.h>
#include "Components.hpp"
#include <onut/Renderer.h>

void renderGears(entt::registry const& registry)
{
    auto texture = OGetTexture("gear.png");
    auto view = registry.view<Machine const, Gear const>();
    for (auto entity : view)
    {
        auto const& machine = view.get<Machine const>(entity);
        auto const& gear = view.get<Gear const>(entity);

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

void renderDurabilityBar(entt::registry const& registry)
{
    auto view = registry.view<Machine const, Durability const>();

    for (auto entity : view)
    {
        auto const& machine = view.get<Machine const>(entity);
        auto durability = view.get<Durability const>(entity).durability;

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

void Renderer::run()
{
    oRenderer->clear(OColorHex(556677));
    oSpriteBatch->begin();

    auto& registry = state_.entities;
    renderGears(registry);
    renderDurabilityBar(registry);
    renderRepairiumBar(state_);

    oSpriteBatch->end();
}