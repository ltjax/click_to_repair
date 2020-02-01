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
  anim_current_level_.play(
    0.05f, // From
    0.2f, // To
    0.4f, // Duration in Seconds
    OTweenEaseBoth,
    OPingPongLoop
  );
  anim_main_logo_.play(
    0.f, // From
    20.f, // To
    5.f, // Duration in Seconds
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
    oSpriteBatch->drawSprite(icon_wrench, Vector2(OScreenCenterXf, OScreenHf / 3.f),
      Color::White, anim_main_logo_ - 15.f, targetSize / textureSize.x, onut::Align::Center);
  }

  {
    const auto size_per_level = OScreenWf / Constants::MAX_LEVELS();

    for (int level = 0; level < Constants::MAX_LEVELS(); ++level)
    {
      auto rect = get_level_box(level);
      Color color = rect.Contains(oInput->mousePosf) ? Color::White : Color{ 0.9f,0.9f,0.9f,1.f };

      if (level == progress.next_available_level)
      {
        const auto size = get_level_box_size();
        auto diff = (anim_current_level_.get() - 0.1f) * size * LEVEL_BOX_SIZE;
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

  {
    auto texture_connect = OGetTexture("white_dot.png");
    auto texture_connect_next = OGetTexture("quality_circle.png");
    const auto size_per_level = OScreenWf / Constants::MAX_LEVELS();
    auto padding = 32.f;
    auto offset = Vector2{ padding, 0.f };

    for (int level = 0, level_end = std::min(progress.next_available_level, Constants::MAX_LEVELS()-1); level < level_end; ++level)
    {
      auto rect = get_level_box(level);
      auto connect = level == progress.next_available_level -1 ? texture_connect_next : texture_connect;
      Vector2 connect_pos = rect.Right() + offset;
      auto stop = (get_level_box(level + 1).Left() - offset).x;
      auto dist = stop - connect_pos.x;
      auto num_dots = std::min(3, static_cast<int>(dist / padding));
      connect_pos.x+= (dist - num_dots * padding) / 2.f + padding / 2.f;
      for (int dot = 0; dot < num_dots; ++dot)
        oSpriteBatch->drawSprite(connect, connect_pos + offset * dot, Color::White, onut::Align::Center);
    }
  }

  oSpriteBatch->end();
}
