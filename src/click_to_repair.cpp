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

class GearComponent : public OComponent
{
public:
    GearComponent() : OComponent(FLAG_RENDERABLE_2D)
    {
        mTexture = OGetTexture("gear.png");
    }

    void setSize(float size)
    {
        m_size = size;
    }

private:
    void onRender2d() override
    {
        auto& transform = getEntity()->getWorldTransform();
        oSpriteBatch->drawSprite(mTexture, transform, Vector2(m_size));
    }


    OTextureRef mTexture;
    float m_size = 64.0f;
};

void initSettings()
{
    oSettings->setGameName("Click to repair!");
}

void init()
{
    auto gear = OEntity::create();
    gear->addComponent<GearComponent>();
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