#include "Levels.hpp"
#include "Components.hpp"
#include "Constants.hpp"
#include <onut/Renderer.h>

entt::entity createGear(entt::registry& registry, Vector2 position)
{
    auto gear = registry.create();
    registry.assign<Machine>(gear, position, 128.f);
    registry.assign<QualityStatus>(gear);
    registry.assign<Gear>(gear);
    registry.assign<Durability>(gear, 1.f);
    registry.assign<HoverSound>(gear, OCreateSoundInstance("aircompressor.wav"));
    registry.assign<HoverState>(gear);
    return gear;
}

entt::entity createEngine(entt::registry& registry, Vector2 position)
{
  auto engine = registry.create();
  registry.assign<Machine>(engine, position, 128.f);
  registry.assign<Engine>(engine);
  registry.assign<Durability>(engine, 1.f);
  registry.assign<QualityStatus>(engine);
  return engine;
}

void level0(entt::registry& registry, LevelData& state)
{
    createGear(registry, Vector2{0.f});
}

void level1(entt::registry& registry, LevelData& state)
{
    createGear(registry, Vector2(-150, 0));
    createGear(registry, Vector2(150, 0));
}

void level2(entt::registry& registry, LevelData& state)
{
    createGear(registry, Vector2(-150, 0));
    createEngine(registry, Vector2(150, 0));
}

using LevelFunc = void (*)(entt::registry&, LevelData&);
LevelFunc levels[] = {
    level0,
    level1,
    level2,
};
constexpr auto MAX_LEVEL = std::size(levels);
namespace Constants
{
  int MAX_LEVELS()
  {
    return MAX_LEVEL;
  }
}

void setup_level(LevelData& level, int level_index)
{
    assert(level_index < MAX_LEVEL);
    levels[level_index](level.entities, level);
}

LevelData create_level(int index)
{
    LevelData result;
    setup_level(result, index);
    return result;
}
