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

class MachineSize : public OComponent
{
public:
    float size = 128.f;
};

class GearComponent : public OComponent
{
public:
    GearComponent() : OComponent(FLAG_RENDERABLE_2D|FLAG_UPDATABLE)
    {
        mTexture = OGetTexture("gear.png");
        mTransform = Matrix::Identity;
    }

private:
    void onRender2d() override
    {
        auto size = getComponent<MachineSize>()->size;

        auto transform = mTransform * getEntity()->getWorldTransform();
        auto textureSize = mTexture->getSizef();
        oRenderer->renderStates.blendMode = OBlendAlpha;
        oSpriteBatch->drawSprite(mTexture, transform, Vector2(size / textureSize.x, size / textureSize.y));
    }

    void onUpdate() override
    {
        mTransform = Matrix::CreateRotationZ(ODT) * mTransform;
    }


    OTextureRef mTexture;
    Matrix mTransform;
};

class DurabilityComponent : public OComponent
{
public:
    DurabilityComponent() : OComponent(FLAG_RENDERABLE_2D|FLAG_UPDATABLE)
    {
    }


private:
    void onRender2d() override
    {
        auto transform = getEntity()->getWorldTransform();
        
        auto p = Vector4::Transform(Vector4{0.f, 0.f, 0.f, 1.f}, transform);
        
        auto size = getComponent<MachineSize>()->size;
        auto halfSize = size * 0.5f;
        auto rect = Rect{-halfSize + p.x, -halfSize + p.y - 8, size * mDurability, 8};
        oSpriteBatch->drawRect(nullptr, rect, OColorHex(FF0000));
    }
    
    void onUpdate() override
    {
        if (OInputPressed(OMouse1))
        {
            mDurability = min(1.f, mDurability + ODT * 0.15f);
        }
        else
        {
            mDurability = max(0.f, mDurability - ODT * 0.05f);
        }
    }

    // 1 is full
    float mDurability = 1.f;
};

void initSettings()
{
    oSettings->setGameName("Click to repair!");
}

void init()
{
    oContentManager->addSearchPath("../../../../assets");
    auto gear = OEntity::create();
    gear->addComponent<MachineSize>();
    gear->addComponent<GearComponent>();
    gear->addComponent<DurabilityComponent>();
    gear->setLocalTransform(Matrix::CreateTranslation(OScreenCenterf / 2.0f));
}

void update()
{
  if (OInputJustPressed(OKeyEscape))
    OQuit();
    
}

void render()
{
    oRenderer->clear(OColorHex(556677));
}

void postRender()
{
}

void renderUI()
{
}