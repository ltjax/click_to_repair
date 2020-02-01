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

void updateDuration(GameState& state, std::chrono::duration<float> dt)
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
            state.repairium = std::max(0.f, state.repairium - dt.count() * 0.2f);
        }
        else
        {
            durability = std::max(0.f, durability - dt.count() * 0.05f);
        }
    }
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
    state.quality = quality / view.size();
}

void updateRepairum(GameState& state, std::chrono::duration<float> dt)
{
    if (state.quality < 0.1)
    {
        // machine is stopped
        return;
    }

    if (state.quality >= 0.8)
    {
        // machine is in SUPER condition and run
        state.repairium += 0.1 * dt.count();
    }

    // normal condition
    state.repairium += 0.05 * state.quality * dt.count();
}

void Updater::run(std::chrono::duration<float> dt)
{
    auto& registry = state_.entities;
    updateGears(registry, dt);
    updateDuration(state_, dt);
    updateQuality(state_, dt);
    updateRepairum(state_, dt);
    
    if (OInputJustPressed(OKeyEscape))
        OQuit();
}
