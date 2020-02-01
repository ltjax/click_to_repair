#include "Updater.hpp"
#include "Components.hpp"
#include "Levels.hpp"
#include <onut/Input.h>
#include <onut/onut.h>
#include "Constants.hpp"
#include <onut/Sound.h>
#include <random>

void updateGears(entt::registry& registry, std::chrono::duration<float> dt)
{
    auto view = registry.view<Gear>();
    for (auto entity : view)
    {
        auto& gear = view.get<Gear>(entity);
        gear.rotation = Matrix::CreateRotationZ(dt.count()) * gear.rotation;
        gear.delta += dt.count();
    }
}

void updateEngines(entt::registry& registry, std::chrono::duration<float> dt)
{
    auto view = registry.view<Engine>();
    for (auto entity : view)
    {
        auto& engine = view.get<Engine>(entity);
        engine.cam_shaft_angle += dt.count();
    }
}

void updateDurability(LevelData& state, std::chrono::duration<float> dt)
{
    entt::registry& registry = state.entities;
    auto view = registry.view<Durability, Machine>();
    bool was_repairing = state.is_repairing;
    state.started_repairing = false;
    state.is_repairing = false;
    for (auto entity : view)
    {
        auto const& machine = view.get<Machine>(entity);
        auto& durability = view.get<Durability>(entity).durability;
        if (OInputPressed(OMouse1) && machine.getBoundingBox().Contains(oInput->mousePosf))
        {
            auto speed = state.repairium > 0.f ?
                Constants::REPAIR_DURATION_INCREASE_PER_SECOND_WITH_RESOURCE :
                Constants::REPAIR_DURATION_INCREASE_PER_SECOND_WHEN_EMPTY;

            durability = std::min(1.f, durability + dt.count() * speed);

            state.is_repairing = true;
        }
    }

    if (!was_repairing)
    {
        state.started_repairing = true;
    }
}

void updateWear(LevelData& state, std::chrono::duration<float> dt)
{
    if (state.is_repairing)
    {
        return;
    }

    entt::registry& registry = state.entities;
    auto view = registry.view<Durability, Machine>();
    bool was_repairing = state.is_repairing;
    state.started_repairing = false;
    state.is_repairing = false;
    for (auto entity : view)
    {
        auto& durability = view.get<Durability>(entity).durability;
        durability = std::max(0.f, durability - dt.count() * Constants::WEAR_PER_SECOND);
    }
}

Quality computeQuality(float durability, float lower, float upper )
{
    if (durability < lower)
    {
        return Quality::Worst;
    }

    if (durability >= upper)
    {
        return Quality::Good;
    }
    return Quality::Medium;
}

void updateGlobalQuality(LevelData& state, std::chrono::duration<float> dt)
{
    entt::registry& registry = state.entities;
    auto view = registry.view<Durability>();

    float min_durability = 1.f;
    for (auto entity : view)
    {
        auto& durability = view.get<Durability>(entity).durability;

        min_durability = std::min(min_durability, durability);
    }

    state.quality = computeQuality(min_durability, 0.1, 0.8);
}

void updateGearQuality(LevelData& state, std::chrono::duration<float> dt)
{
    entt::registry& registry = state.entities;
    auto view = registry.view<Gear, Durability, Quality>();

    for (auto entity : view)
    {
        auto& durability = view.get<Durability>(entity).durability;
        auto& quality = view.get<Quality>(entity);

        quality = computeQuality(durability, 0.1, 0.5);
    }
}


void updateRepairTime(LevelData& state, std::chrono::duration<float> dt)
{
    static std::mt19937 Rng;

    if (state.is_repairing)
    {
        auto previous = state.repair_time;
        state.repair_time += dt.count();

        auto sound_period = 0.5f;
        if (std::ceil(previous / sound_period) != std::ceil(state.repair_time / sound_period))
        {
            std::uniform_int_distribution<int> SoundIndex(1, 7);
            OPlaySound("wrench_" + std::to_string(SoundIndex(Rng)) + ".wav", 1.f, 0.f, 1.f);
        }

        return; // No new repairium if we repair
    }
    else
    {
        state.repair_time = 0.f;
    }
}

void updateRepairum(LevelData& state, std::chrono::duration<float> dt)
{
    if (state.is_repairing)
    {
        if (state.started_repairing)
        {
            state.repairium = std::max(0.f, state.repairium - Constants::REPAIR_UP_FRONT_COST);
        }
        state.repairium = std::max(0.f, state.repairium - dt.count() * Constants::REPAIR_RESOURCE_DRAIN_PER_SECOND);
        return; // No new repairium if we repair
    }

    if (state.quality == Quality::Worst)
    {
        // machine is stopped
        return;
    }

    if (state.quality == Quality::Good)
    {
        // machine is in SUPER condition and run
        state.repairium += 0.1f * dt.count();
        return;
    }

    // normal condition
    state.repairium += 0.05f * dt.count();
}

std::optional<GameFinished> Updater::run(std::chrono::duration<float> dt)
{
    auto& registry = level.entities;
    updateGears(registry, dt);
    updateEngines(registry, dt);
    updateDurability(level, dt);
    updateWear(level, dt);
    updateGearQuality(level, dt);
    updateGlobalQuality(level, dt);
    updateRepairTime(level, dt);
    updateRepairum(level, dt);
    
    if (level.repairium >= 1.0f)
    {
        level.repairium = 1.f;
        level.won_level = true;
    }

    if (level.won_level)
    {
        return GameFinished{};
    }

    return {};
}
