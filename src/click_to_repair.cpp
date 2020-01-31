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

void initSettings()
{
    oSettings->setGameName("Click to repair!");
}

void init()
{
}

void update()
{
  if (OInputJustPressed(OKeyEscape))
    OQuit();
    
}

void render()
{
    oRenderer->clear(OColorHex(FFFFFF));

}

void postRender()
{
}

void renderUI()
{
}