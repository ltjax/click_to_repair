#include "MainMenuScreen.hpp"
#include "InGameScreen.hpp"
#include "Constants.hpp"
#include <onut/Input.h>
#include <onut/onut.h>
#include <onut/Renderer.h>
#include <onut/Texture.h>
#include <onut/Window.h>

namespace {
  constexpr float LEVEL_BOX_SIZE = 0.2f;
  constexpr float LEVEL_BOX_OFFSET = (1.f - LEVEL_BOX_SIZE) / 2.f;

  float get_level_box_size() {
    return std::min(OScreenWf / Constants::MAX_LEVELS(), OScreenHf / 3.f);
  }
  Rect get_level_box(int level_index) {
    const auto size_per_level = get_level_box_size();
    const auto initial_offset = (OScreenWf - size_per_level * Constants::MAX_LEVELS()) / 2.f;

    return { initial_offset + (level_index + LEVEL_BOX_OFFSET) * size_per_level, OScreenHf - (LEVEL_BOX_OFFSET + LEVEL_BOX_SIZE ) * size_per_level,
      size_per_level * LEVEL_BOX_SIZE, size_per_level * LEVEL_BOX_SIZE };
  }
}

MainMenuScreen::MainMenuScreen(Progress& progress_)
  : progress(progress_)
{
  anim_.play(
    0.05f, // From
    0.2f, // To
    0.4f, // Duration in Seconds
    OTweenEaseBoth,
    OPingPongLoop
  );
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
    for (int level = 0; level <= progress.next_available_level; ++level)
      if (get_level_box(level).Contains(oInput->mousePosf))
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

  {
    const auto size_per_level = OScreenWf / Constants::MAX_LEVELS();
    for (int level = 0; level < Constants::MAX_LEVELS(); ++level)
    {
      auto rect = get_level_box(level);
      //Color color = rect.Contains(oInput->mousePosf) ? Color{ 0.7,1.f,0.7,1.f } : Color::White;
      Color color = rect.Contains(oInput->mousePosf) ? Color::White : Color{ 0.9f,0.9f,0.9f,1.f };
      if (level == progress.next_available_level)
      {
        const auto size = get_level_box_size();
        auto diff = (anim_.get() - 0.1f) * size * LEVEL_BOX_SIZE;
        rect.x += diff;
        rect.y += diff;
        rect.z -= 2 * diff;
        rect.w -= 2 * diff;
      }
      else if (level < progress.next_available_level)
      {
        const auto size = get_level_box_size();
        auto diff = 0.2f * size * LEVEL_BOX_SIZE;
        auto small_rect = rect;
        small_rect.x += diff;
        small_rect.y += diff;
        small_rect.z -= 2 * diff;
        small_rect.w -= 2 * diff;
        oSpriteBatch->drawRect(nullptr, small_rect, color);
      }
      else {
        color = Color{ 0.5f, 0.5f, 0.5f, 1.f };
      }
      oSpriteBatch->drawOutterOutlineRect(rect, 5.f, color);
    }
  }

  oSpriteBatch->end();
}
