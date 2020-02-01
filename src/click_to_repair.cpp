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
            durability = std::min(1.f, durability + dt.count() * 0.15f);
        }
        else
        {
            durability = std::max(0.f, durability - dt.count() * 0.05f);
        }
    }
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
        auto contentRect = shrinkRect(backgroundRect, Vector2{1});
        contentRect.z *= durability;
        oSpriteBatch->drawRect(nullptr, backgroundRect, Color::White);

        auto color = OColorRGB(88,88,88);
        if (durability < 0.1f)
        {
            color = OColorRGB(220, 20, 20);
        }
        else if (durability < 0.5f)
        {
            color = OColorRGB(220, 220, 20);
        }
        oSpriteBatch->drawRect(nullptr, contentRect, color);
    }
}


void update()
{
    auto dt = std::chrono::duration<float>{ODT};
    updateGears(oRegistry, dt);
    updateDuration(oRegistry, dt);

    if (OInputJustPressed(OKeyEscape))
        OQuit();

}

void render()
{
    oRenderer->clear(OColorHex(556677));
    oSpriteBatch->begin();
    renderGears(oRegistry);
    renderDurabilityBar(oRegistry);
    oSpriteBatch->end();
}

void postRender()
{
}

void renderUI()
{
}