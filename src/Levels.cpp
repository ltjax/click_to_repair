#include "Levels.hpp"
#include "Components.hpp"
#include "Constants.hpp"
#include <onut/Renderer.h>

entt::entity createGear(entt::registry& registry, Vector2 position, float durability)
{
    auto gear = registry.create();
    registry.assign<Machine>(gear, position, 128.f);
    registry.assign<QualityStatus>(gear);
    registry.assign<Gear>(gear);
    registry.assign<Durability>(gear, durability);
    registry.assign<HoverSound>(gear, OCreateSoundInstance("aircompressor.wav"));
    registry.assign<HoverState>(gear);
    return gear;
}

entt::entity createEngine(entt::registry& registry, Vector2 position, float durability)
{
    auto engine = registry.create();
    registry.assign<Machine>(engine, position, 128.f);
    registry.assign<QualityStatus>(engine);
    registry.assign<Engine>(engine);
    registry.assign<Durability>(engine, durability);
    registry.assign<HoverState>(engine);
    return engine;
}

entt::entity createHamster(entt::registry& registry, Vector2 position, float durability)
{
    auto hamster = registry.create();
    registry.assign<Machine>(hamster, position, 128.f);
    registry.assign<Hamster>(hamster);
    registry.assign<Durability>(hamster, durability);
    registry.assign<HoverState>(hamster);
    registry.assign<QualityStatus>(hamster);
    registry.assign<HoverSound>(hamster, OCreateSoundInstance("hamster.wav"));
    return hamster;
}

void level0(entt::registry& registry, LevelData& state)
{
    createGear(registry, Vector2{0.f}, 0.4f);
}

void level1(entt::registry& registry, LevelData& state)
{
    createGear(registry, Vector2(-150, 0), 0.65f);
    createGear(registry, Vector2(150, 0), 0.65f);
}

void level2(entt::registry& registry, LevelData& state)
{
    createGear(registry, Vector2(-150, 0), 0.4f);
    createEngine(registry, Vector2(150, 0), 1.f);
}

void level3(entt::registry& registry, LevelData& state)
{
    createEngine(registry, Vector2(-300.f, 0.f), 1.f);
    createHamster(registry, Vector2(0.f, 0.f), 0.75f);
    createEngine(registry, Vector2(300.f, 0.f), 1.f);
}

void level4(entt::registry& registry, LevelData& state)
{
    float x = 160.f;
    float y = 125.f;
    createEngine(registry, Vector2(-x, -y), 1.f);
    createGear(registry, Vector2(-x, y), 0.75f);
    createGear(registry, Vector2(x, -y), 0.75f);
    createHamster(registry, Vector2(x, y), 1.0f);
    state.reparium_multiplier = 1.6f;
}

using LevelFunc = void (*)(entt::registry&, LevelData&);
LevelFunc levels[] = {
    level0,
    level1,
    level2,
    level3,
    level4,
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
