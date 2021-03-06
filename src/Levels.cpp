#include "Levels.hpp"
#include "Components.hpp"
#include "Constants.hpp"
#include <onut/Renderer.h>

entt::entity createPart(entt::registry& registry, Vector2 position, float durability, std::string const& hoverSound)
{
    auto part = registry.create();
    registry.assign<Machine>(part, position, 128.f);
    registry.assign<QualityStatus>(part);
    registry.assign<Durability>(part, durability);
    registry.assign<HoverSound>(part, OCreateSoundInstance(hoverSound));
    registry.assign<HoverState>(part);
    return part;
}

entt::entity createGear(entt::registry& registry, Vector2 position, float durability)
{
    auto gear = createPart(registry, position, durability, "cogs.wav");
    registry.assign<Gear>(gear);
    return gear;
}

entt::entity createFluxCapacitor(entt::registry& registry, Vector2 position, float durability) {
    auto flux = createPart(registry, position, durability, "flux_capacitor.wav");
    registry.assign<FluxCapacitor>(flux);
    registry.assign<Overload>(flux);
    return flux;
}

entt::entity createEngine(entt::registry& registry, Vector2 position, float durability)
{
    auto engine = createPart(registry, position, durability, "ambient_machine1.wav");
    registry.assign<Engine>(engine);
    registry.assign<EngineSFX>(engine, OCreateSoundInstance("engine_shake.wav"));
    return engine;
}

entt::entity createHamster(entt::registry& registry, Vector2 position, float durability)
{
    auto hamster = createPart(registry, position, durability, "hamster.wav");
    registry.assign<Hamster>(hamster);
    registry.assign<Hiccup>(hamster, "hiccup.wav");
    return hamster;
}

void level0(entt::registry& registry, LevelData& state)
{
    createGear(registry, Vector2{0.f}, 0.2f);
}

void level1(entt::registry& registry, LevelData& state)
{
    createGear(registry, Vector2(-150, 0), 0.65f);
    createGear(registry, Vector2(150, 0), 0.75f);
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
    state.reparium_multiplier = 1.25f;
}

void level5(entt::registry& registry, LevelData& state)
{
    createEngine(registry, Vector2(-300.f, 0.f), 1.f);
    createFluxCapacitor(registry, Vector2(0.f, 0.f), 0.5f);
    createHamster(registry, Vector2(300.f, 0.f), 0.75f);
}

void level6(entt::registry& registry, LevelData& state)
{
    float x = 160.f;
    float y = 125.f;
    createGear(registry, Vector2(-x, -y), 1.f);
    createFluxCapacitor(registry, Vector2(-x, y), 0.75f);
    createFluxCapacitor(registry, Vector2(x, -y), 0.75f);
    createHamster(registry, Vector2(x, y), 1.0f);
}

using LevelFunc = void (*)(entt::registry&, LevelData&);
LevelFunc levels[] = {
    level0,
    level1,
    level2,
    level3,
    level4,
    level5,
    level6,
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
