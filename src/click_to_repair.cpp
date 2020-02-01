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

struct Durability
{
    float durability = 1.f;
};

struct Machine
{
    Vector2 position;
    float size = 128.f;
};

struct Gear
{
    Matrix rotation;
};

class MachineSize : public OComponent
{
public:
    float size = 128.f;

    Rect getBoundingBox() const
    {
        auto transform = getEntity()->getWorldTransform();

        auto p = Vector4::Transform(Vector4{ 0.f, 0.f, 0.f, 1.f }, transform);

        auto size = getComponent<MachineSize>()->size;
        auto halfSize = size * 0.5f;
        return Rect{ -halfSize + p.x, -halfSize + p.y, size, size };
    }
};

class DurabilityComponent : public OComponent
{
public:
    DurabilityComponent() : OComponent(FLAG_RENDERABLE_2D | FLAG_UPDATABLE)
    {
    }


private:
    void onRender2d() override
    {
        auto transform = getEntity()->getWorldTransform();

        auto p = Vector4::Transform(Vector4{ 0.f, 0.f, 0.f, 1.f }, transform);

        auto size = getComponent<MachineSize>()->size;
        auto halfSize = size * 0.5f;
        auto rect = Rect{ -halfSize + p.x, -halfSize + p.y - 8, size * mDurability, 8 };
        oSpriteBatch->drawRect(nullptr, rect, OColorHex(FF0000));
    }

    void onUpdate() override
    {
        // oInput->mousePosf
        if (OInputPressed(OMouse1) && getComponent<MachineSize>()->getBoundingBox().Contains(oInput->mousePosf))
        {
            mDurability = std::min(1.f, mDurability + ODT * 0.15f);
        }
        else
        {
            mDurability = std::max(0.f, mDurability - ODT * 0.05f);
        }
    }

    // 1 is full
    float mDurability = 1.f;
};

entt::entity createGear(entt::registry& registry)
{
    auto gear = registry.create();
    registry.assign<Machine>(gear, Vector2{0.f}, 128.f);
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

    oRegistry.get<Machine>(createGear(oRegistry)).position = OScreenCenterf / 2.0f;    
    oRegistry.get<Machine>(createGear(oRegistry)).position = OScreenCenterf + OScreenCenterf / 2.0f;
}

void updateGears(entt::registry& registry)
{
    auto view = registry.view<Gear>();
    for (auto entity : view)
    {
        auto& gear = view.get<Gear>(entity);        
        gear.rotation = Matrix::CreateRotationZ(ODT) * gear.rotation;
    }
}

void renderGears(entt::registry& registry)
{
    auto texture = OGetTexture("gear.png");
    auto view = registry.view<Machine, Gear>();
    for (auto entity : view)
    {
        auto& machine = view.get<Machine>(entity);
        auto& gear = view.get<Gear>(entity);
        

        auto transform = gear.rotation * Matrix::CreateTranslation(machine.position.x, machine.position.y, 0.f);
        auto textureSize = texture->getSizef();
        oRenderer->renderStates.blendMode = OBlendAlpha;
        oSpriteBatch->drawSprite(texture, transform, Vector2(machine.size / textureSize.x, machine.size / textureSize.y));
    }
}


void update()
{
    updateGears(oRegistry);

    if (OInputJustPressed(OKeyEscape))
        OQuit();

}

void render()
{
    oRenderer->clear(OColorHex(556677));
    oSpriteBatch->begin();
    renderGears(oRegistry);
    oSpriteBatch->end();
}

void postRender()
{
}

void renderUI()
{
}