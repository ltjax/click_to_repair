#include "MainMenuScreen.hpp"
#include "InGameScreen.hpp"
#include "Constants.hpp"
#include "utils.hpp"
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
  Rect get_exit_rect()
  {
    Vector2 size(96);
    return { OScreenWf - size.x, 0, size };
  }
}

MainMenuScreen::MainMenuScreen(std::shared_ptr<SharedState> sharedState_)
  : sharedState(sharedState_)
{
  anim_current_level_.play(
    0.05f, // From
    0.2f, // To
    0.4f, // Duration in Seconds
    OTweenEaseBoth,
    OPingPongLoop
  );
  anim_mouse_.play(
    -0.7f, // From
    0.3f, // To
    0.5f, // Duration in Seconds
    OTweenLinear,
    OPingPongLoop
  );
  anim_arrow_.play(
    -0.5f, // From
    0.5f, // To
    2.f, // Duration in Seconds
    OTweenEaseOut,
    OPingPongLoop
  );
  anim_main_logo_.play(
    0.f, // From
    20.f, // To
    5.f, // Duration in Seconds
    OTweenEaseBoth,
    OPingPongLoop
  );
  backgroundMusic = OGetMusic("menu_music.ogg");
  backgroundMusic->play();
}

Screen::Factory MainMenuScreen::update(std::chrono::duration<float> dt)
{
  if (OInputJustPressed(OKeyEscape))
  {
      OQuit();
      return {};
  }

  if (OInputJustPressed(OMouse1))
  {
      auto const& progress = sharedState->progress;
      for (int level = 0; level <= progress.next_available_level; ++level)
          if (get_level_box(level).Contains(oInput->mousePosf))
              return [sharedState = sharedState, level = level]() {return std::make_unique<InGameScreen>(sharedState, level);};

    if (get_exit_rect().Contains(oInput->mousePosf))
      OQuit();
  }

  return {};
}

void MainMenuScreen::render()
{
  oRenderer->clear(Constants::BackgroundColor());

  oSpriteBatch->begin();
  {
    auto icon_wrench = OGetTexture("mouse.png");
    auto textureSize = icon_wrench->getSizef();
    const float targetSize = 0.7f * OScreenHf * 2.f / 3.f;
    auto pos = Vector2(OScreenCenterXf * 0.5f, OScreenHf / 3.f + 32.f);
    oSpriteBatch->drawSprite(icon_wrench, pos,
      Color::White, 0.f,  targetSize / textureSize.x, onut::Align::Center);
    if (anim_mouse_ > 0.f)
    {
        drawCircleFill(pos + Vector2(-targetSize / 10.f, -targetSize / 15.f), Color::White, 20.f);
    }
  }
  {
    auto icon_wrench = OGetTexture("back.png");
    auto textureSize = icon_wrench->getSizef();
    const float targetSize = 0.3f * OScreenHf * 2.f / 3.f;
    oSpriteBatch->drawSprite(icon_wrench, Vector2(OScreenCenterXf + anim_arrow_ * 0.05f * OScreenWf, OScreenHf *5.f / 12.f ),
      Color::White, 180.f, targetSize / textureSize.x, onut::Align::Center);
  }
  {
    auto icon_wrench = OGetTexture("wrench.png");
    auto textureSize = icon_wrench->getSizef();
    const float targetSize = 0.7f * OScreenHf * 2.f / 3.f;
    oSpriteBatch->drawSprite(icon_wrench, Vector2(OScreenCenterXf * 1.5f, OScreenHf / 3.f + 32.f),
      Color::White, anim_main_logo_ - 15.f, targetSize / textureSize.x, onut::Align::Center);
  }

  {
    const auto size_per_level = OScreenWf / Constants::MAX_LEVELS();

    auto& progress = sharedState->progress;
    for (int level = 0; level < Constants::MAX_LEVELS(); ++level)
    {
      auto rect = get_level_box(level);
      Color color = get_color_focus(rect);

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
        color = Color::White;
      }
      oSpriteBatch->drawOutterOutlineRect(rect, 5.f, color);
    }
  }

  {
    const auto size_per_level = OScreenWf / Constants::MAX_LEVELS();
    auto padding = 32.f;
    auto offset = Vector2{ padding, 0.f };

    auto& progress = sharedState->progress;
    for (int level = 0, level_end = std::min(progress.next_available_level, Constants::MAX_LEVELS()-1); level < level_end; ++level)
    {
      auto rect = get_level_box(level);
      Vector2 connect_pos = rect.Right() + offset;
      auto stop = (get_level_box(level + 1).Left() - offset).x;
      auto dist = stop - connect_pos.x;
      auto num_dots = std::min(3, static_cast<int>(dist / padding));
      connect_pos.x+= (dist - num_dots * padding) / 2.f + padding / 2.f;
      for (int dot = 0; dot < num_dots; ++dot)
      {
        if (level == progress.next_available_level -1 )
            drawCircleFill(connect_pos+offset*dot, Color::White, 10.f);
        else
            drawCircle(connect_pos+offset*dot, Color::White, 10.f, 4.f);
      }
    }
  }

  {
    oSpriteBatch->drawRect(OGetTexture("exit.png"), get_exit_rect(), get_color_focus(get_exit_rect()));
  }

  oSpriteBatch->end();
}
