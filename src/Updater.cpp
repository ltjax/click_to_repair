#include "Updater.hpp"
#include "Components.hpp"
#include <onut/Input.h>
#include <onut/onut.h>
#include "Constants.hpp"

void updateGears(entt::registry& registry, std::chrono::duration<float> dt)
{
    auto view = registry.view<Gear>();
    for (auto entity : view)
    {
        auto& gear = view.get<Gear>(entity);
        gear.rotation = Matrix::CreateRotationZ(dt.count()) * gear.rotation;
    }
}

void updateDurability(GameState& state, std::chrono::duration<float> dt)
{
    entt::registry& registry = state.entities;
    auto view = registry.view<Durability, Machine>();
    state.started_repairing = false;
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

            if (!state.is_repairing)
            {
                state.started_repairing = true;
            }
            state.is_repairing = true;
        }
        else
        {
            durability = std::max(0.f, durability - dt.count() * Constants::WEAR_PER_SECOND);
            state.is_repairing = false;
        }
    }
}

Quality computeQuality(float averageDurability)
{
    if (averageDurability < 0.1)
    {
        return Quality::Worst;
    }

    if (averageDurability >= 0.8)
    {
        return Quality::Good;
    }
    return Quality::Medium;
}

void updateQuality(GameState& state, std::chrono::duration<float> dt)
{
    entt::registry& registry = state.entities;
    auto view = registry.view<Durability>();

    float quality = 0.f;
    for (auto entity : view)
    {
        auto& durability = view.get<Durability>(entity).durability;

        quality += durability;
    }

    auto averageDurability = quality / view.size();
    state.quality = computeQuality(averageDurability);
}

void updateRepairum(GameState& state, std::chrono::duration<float> dt)
{
    if (state.is_repairing)
    {
        if (state.started_repairing)
        {
            state.repairium = std::max(0.f, state.repairium - Constants::REPAIR_UP_FRONT_COST);
        }
        state.repairium = std::max(0.f, state.repairium - dt.count() * Constants::REPAIR_RESOURCE_DRAIN_PER_SECOND);
        state.repair_time += dt.count();
        return; // No new repairium if we repair
    }
    else
    {
        state.repair_time = 0.f;
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

    if (state.repairium >= 1.f)
    {
        // TODO: Win condition here!
        state.repairium = 1.f;
    }
}

void Updater::run(std::chrono::duration<float> dt)
{
    auto& registry = state_.entities;
    updateGears(registry, dt);
    updateDurability(state_, dt);
    updateQuality(state_, dt);
    updateRepairum(state_, dt);
    
    if (OInputJustPressed(OKeyEscape))
        OQuit();
}
