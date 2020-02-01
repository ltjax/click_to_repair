#include "Updater.hpp"
#include "Components.hpp"
#include <onut/Input.h>
#include <onut/onut.h>

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

    for (auto entity : view)
    {
        auto const& machine = view.get<Machine>(entity);
        auto& durability = view.get<Durability>(entity).durability;
        if (OInputPressed(OMouse1) && machine.getBoundingBox().Contains(oInput->mousePosf))
        {
            auto speed = state.repairium > 0.f ? 0.15f : 0.03f;
            durability = std::min(1.f, durability + dt.count() * speed);
            state.is_repairing = true;
        }
        else
        {
            durability = std::max(0.f, durability - dt.count() * 0.05f);
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
        state.repairium = std::max(0.f, state.repairium - dt.count() * 0.2f);
        state.repair_time += dt.count();
    }
    else
        state.repair_time = 0.f;

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
