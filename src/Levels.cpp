#include "Levels.hpp"
#include "Components.hpp"
#include <onut/Renderer.h>

entt::entity createGear(entt::registry& registry, Vector2 position)
{
    auto gear = registry.create();
    registry.assign<Machine>(gear, position, 128.f);
    registry.assign<Gear>(gear);
    registry.assign<Durability>(gear, 1.f);
    registry.assign<HoverSound>(gear, OCreateSoundInstance("aircompressor.wav"));
    registry.assign<HoverState>(gear);
    return gear;
}

void level0(entt::registry& registry, LevelData& state)
{
    createGear(registry, OScreenCenterf);
}

void level1(entt::registry& registry, LevelData& state)
{
    createGear(registry, OScreenCenterf);
    createGear(registry, OScreenCenterf + Vector2(200, 0));
}

using LevelFunc = void (*)(entt::registry&, LevelData&);
LevelFunc levels[] = {
    level0,
    level1,
};
constexpr auto MAX_LEVEL = sizeof(levels) / sizeof(levels[0]);

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
