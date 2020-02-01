#include "Renderer.hpp"
#include "utils.hpp"
#include <onut/Texture.h>
#include "Components.hpp"
#include <onut/Renderer.h>
#include <onut/Input.h>
#include "Constants.hpp"

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

        oSpriteBatch->drawSprite(texture, transform, Vector2(machine.size / textureSize.x, machine.size / textureSize.y));
    }
}

Color colorForQuality(Quality quality)
{
    auto color = OColorRGB(255, 255, 255);
    if (quality == Quality::Worst)
    {
        color = OColorRGB(220, 20, 20);
    }
    else if (quality == Quality::Medium)
    {
        color = OColorRGB(220, 220, 20);
    }
    return color;
}


void renderBar(OSpriteBatchRef spriteBatch, Rect rectangle, float fullness, Color color)
{
    auto texture = OGetTexture("bar.png");
    auto contentRect = shrinkRect(rectangle, Vector2{ 2 });
    contentRect.z *= fullness;
    oSpriteBatch->drawRect(nullptr, contentRect, color);
    oSpriteBatch->drawRectScaled9(texture, rectangle, Vector4{ 4.f });
}

void renderMachineFrames(entt::registry const& registry)
{
    auto frame = OGetTexture("frame.png");
    auto view = registry.view<Machine const>();

    for (auto entity : view)
    {
        auto const& machine = view.get<Machine const>(entity);
        oSpriteBatch->drawSprite(frame, machine.position);
    }
}

void renderDurabilityBar(entt::registry const& registry)
{
    auto view = registry.view<Machine const, Durability const, Quality const>();

    for (auto entity : view)
    {
        auto const& machine = view.get<Machine const>(entity);
        auto durability = view.get<Durability const>(entity).durability;
        auto quality = view.get<Quality const>(entity);

        auto p = machine.position;
        auto size = machine.size;
        auto halfSize = size * 0.5f;
        auto barHeight = 10.f;
        auto backgroundRect = Rect{ -halfSize + p.x, -halfSize + p.y - barHeight, size, barHeight };
        renderBar(oSpriteBatch, backgroundRect, durability, colorForQuality(quality));
    }
}

void renderRepairiumBar(LevelData const& state)
{
    auto barHeight = 30.f;
    auto screenSize = OScreenf;
    auto rect = Rect{ 0.f, screenSize.y - barHeight, screenSize.x, barHeight };
    rect = shrinkRect(rect, Vector2{ 8.f });
    renderBar(oSpriteBatch, rect, state.repairium, Color::White);
}

void renderQualityLights(LevelData const& state)
{
    auto screenSize = OScreenf;
    auto texture = OGetTexture("white_dot.png");
    auto circle = OGetTexture("quality_circle.png");
    auto padding = 32.f;

    auto offset = Vector2{ padding, 0.f };
    Vector2 positionCenter{ screenSize.x / 2.f, texture->getSizef().y / 2.f + padding };
    Vector2 positionLeft = positionCenter - offset;
    Vector2 positionRight = positionCenter + offset;

    auto quality = state.quality;

    struct { Vector2 position; Color color; } lights[]{
        {positionLeft, quality == Quality::Worst ? OColorRGB(255, 0, 0) : OColorRGB(75, 0, 0)},
        {positionCenter, quality == Quality::Medium ? OColorRGB(255, 255, 0) : OColorRGB(75, 75, 0)},
        {positionRight, quality == Quality::Good ? OColorRGB(0, 255, 0) : OColorRGB(0, 75, 0)}
    };

    for (auto const& each : lights)
    {
        auto transform = Matrix::CreateTranslation(each.position);
        oSpriteBatch->drawSprite(texture, transform, each.color);
        oSpriteBatch->drawSprite(circle, transform );
    }
}

void renderCursor(LevelData const& state)
{
    oInput->setMouseVisible(!state.is_repairing);

    if (!state.is_repairing)
        return;

    auto icon_wrench = OGetTexture("wrench.png");
    auto textureSize = icon_wrench->getSizef();

    const float targetSize = 96;
    static float rot_time;
    auto s = std::sin(state.repair_time * 7.f);
    float rotation = (s < 0 ? 1.f : -1.f) * (s * s) * 25.f;
    oSpriteBatch->drawSprite(icon_wrench,
        oInput->mousePosf,
        Color::White, rotation, targetSize / textureSize.x,
        Vector2(0.1f, 0.1f));
}


void Renderer::run()
{
    oRenderer->clear(Constants::BackgroundColor());
    oSpriteBatch->begin();

    auto& registry = level.entities;
    oRenderer->renderStates.blendMode = OBlendPreMultiplied;
    renderMachineFrames(registry);
    renderGears(registry);
    renderDurabilityBar(registry);
    renderRepairiumBar(level);
    renderQualityLights(level);
    oRenderer->renderStates.blendMode = OBlendAlpha;
    renderCursor(level); // should be called last, so it is rendered on top

    oSpriteBatch->end();
}
