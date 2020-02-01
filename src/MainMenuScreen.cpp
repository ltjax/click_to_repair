#include "MainMenuScreen.hpp"
#include "InGameScreen.hpp"
#include "Constants.hpp"
#include <onut/Input.h>
#include <onut/onut.h>
#include <onut/Renderer.h>
#include <onut/Texture.h>
#include <onut/Window.h>

namespace {
  constexpr float LEVEL_BOX_SIZE = 0.7f;
  constexpr float LEVEL_BOX_OFFSET = (1.f - LEVEL_BOX_SIZE) / 2.f;

  Rect get_level_box_size(int level_index) {
    const auto size_per_level = std::min(OScreenWf / Constants::MAX_LEVELS(), OScreenHf / 3.f);
    const auto initial_offset = (OScreenWf - size_per_level * Constants::MAX_LEVELS()) / 2.f;

    return { initial_offset + (level_index + LEVEL_BOX_OFFSET) * size_per_level, OScreenHf - (LEVEL_BOX_OFFSET + LEVEL_BOX_SIZE ) * size_per_level,
      size_per_level * LEVEL_BOX_SIZE, size_per_level * LEVEL_BOX_SIZE };
  }
}

std::unique_ptr<Screen> MainMenuScreen::update(std::chrono::duration<float> dt)
{
  if (OInputJustPressed(OKeyEscape))
  {
    OQuit();
    return nullptr;
  }

  if (OInputPressed(OMouse1))
  {
    for (int level=0; level <= progress.next_available_level; ++level)
      if (get_level_box_size(level).Contains(oInput->mousePosf))
        return std::make_unique<InGameScreen>(progress, level);
  }

  return nullptr;
}

void MainMenuScreen::render()
{
  oRenderer->clear(Constants::BackgroundColor());
  oSpriteBatch->begin();
  {
    auto icon_wrench = OGetTexture("wrench.png");
    auto textureSize = icon_wrench->getSizef();
    const float targetSize = OScreenHf * 2.f / 3.f;
    oSpriteBatch->drawSprite(icon_wrench, Vector2(OScreenCenterXf,0),
      Color::White, 0.f, targetSize / textureSize.x, onut::Align::Top);
  }
  const auto size_per_level = OScreenWf / Constants::MAX_LEVELS();
  for (int level = 0; level < Constants::MAX_LEVELS(); ++level)
  {
    
    if (level == progress.next_available_level)
    {
    }
    else if (level < progress.next_available_level)
    {
    }
    else {
    }
    oSpriteBatch->drawOutterOutlineRect(get_level_box_size(level), 5.f, Color::White);
  }

  oSpriteBatch->end();
}
