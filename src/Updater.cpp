#include "Updater.hpp"
#include "Components.hpp"
#include "Levels.hpp"
#include <onut/Input.h>
#include <onut/onut.h>
#include "Constants.hpp"
#include <onut/Sound.h>
#include <random>
#include <onut/Renderer.h>

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
    engine.camShaftAngle += dt.count();
  }
}

void updateHamsters(entt::registry& registry, std::chrono::duration<float> dt)
{
  auto view = registry.view<Hamster>();
  for (auto entity : view)
  {
    auto& hamster = view.get<Hamster>(entity);
    hamster.delta += dt.count();
  }
}

void updateDurability(LevelData& state, std::chrono::duration<float> dt)
{
    entt::registry& registry = state.entities;
    auto view = registry.view<Durability, Machine, HoverState>();
    bool was_repairing = state.is_repairing;
    state.started_repairing = false;
    state.is_repairing = false;
    for (auto entity : view)
    {
        auto const& machine = view.get<Machine>(entity);
        auto& durability = view.get<Durability>(entity).durability;
        auto const& containsMouse = view.get<HoverState>(entity).containsMouse;
        if (OInputPressed(OMouse1) && containsMouse)
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

void updateGearWear(LevelData& state, std::chrono::duration<float> dt)
{
    entt::registry& registry = state.entities;
    auto view = registry.view<Durability, Machine, Gear>();
    for (auto entity : view)
    {
        auto& durability = view.get<Durability>(entity).durability;
        durability = std::max(0.f, durability - dt.count() * Constants::GEAR_WEAR_PER_SECOND);
    }
}

void updateEngineWear(LevelData& state, std::chrono::duration<float> delta)
{
    entt::registry& registry = state.entities;
    auto view = registry.view<Durability, Machine, Engine>();
    for (auto entity : view)
    {
        auto& durability = view.get<Durability>(entity).durability;

        auto wear = view.get<Engine>(entity).shake > 0.f ? 
            Constants::ENGINE_WEAR_PER_SECOND_SHAKING : 
            Constants::ENGINE_WEAR_PER_SECOND;
        durability = std::max(0.f, durability - delta.count() * wear);
    }
}

void updateWear(LevelData& state, std::chrono::duration<float> dt)
{
    if (state.is_repairing)
    {
        return;
    }
    updateGearWear(state, dt);
    updateEngineWear(state, dt);
}

void updateHoverStates(entt::registry& registry, std::chrono::duration<float> dt, Matrix const& camera)
{
    auto view = registry.view<Machine, HoverState>();
    for (auto entity : view)
    {
        auto const& machine = view.get<Machine>(entity);
        auto& hoverState = view.get<HoverState>(entity);
        bool oldState = hoverState.containsMouse;
        hoverState.containsMouse = machine.getBoundingBox(camera).Contains(oInput->mousePosf);
        if (oldState == hoverState.containsMouse) {
            if (hoverState.containsMouse == false)
            {
                hoverState.timeOut += dt;
            }
            else
            {
                hoverState.timeIn += dt;
            }
        }
        else
        {
            if (hoverState.containsMouse == false)
            {
                hoverState.timeOut = std::chrono::duration<float>::zero();
                hoverState.timeIn = std::chrono::duration<float>::zero() + dt;
            }
            else
            {
                hoverState.timeOut = std::chrono::duration<float>::zero() + dt;
                hoverState.timeIn = std::chrono::duration<float>::zero();
            }
        }
    }
}

void updateHoverSounds(entt::registry& registry)
{
    auto view = registry.view<HoverState, HoverSound>();
    for (auto entity : view)
    {
        auto const hoverState = view.get<HoverState>(entity);
        auto const& hoverSound = view.get<HoverSound>(entity);
        if (hoverState.containsMouse)
        {
            if (!hoverSound.background->isPlaying())
            {
                hoverSound.background->setLoop(true);
                hoverSound.background->setVolume(0.0f);
                hoverSound.background->play();
            }
            else 
            {
                auto newVolume = std::clamp(hoverState.timeIn.count(), 0.f, 1.f);
                hoverSound.background->setVolume(newVolume);
            }
        }
        if (!hoverState.containsMouse && hoverSound.background->isPlaying())
        {
            auto newVolume = 1.f - std::clamp(hoverState.timeOut.count(), 0.f, 1.f);
            hoverSound.background->setVolume(newVolume);
            if (hoverState.timeOut.count() > 1.f)
            {
                hoverSound.background->stop();
            }
        }
    }
}

Quality computeQuality(float durability, float lower, float upper)
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


Quality minQuality(Quality first, Quality second)
{
    switch (first) 
    {
        case Quality::Good:
            return second;

        case Quality::Worst:
            return Quality::Worst;

        case Quality::Medium:
            if (second==Quality::Worst)
            {
                return Quality::Worst;
            }
            else
            {
                return Quality::Medium;
            }
    }

    return Quality::Worst;
}

void updateGlobalQuality(LevelData& state, std::chrono::duration<float> dt)
{
    entt::registry& registry = state.entities;
    auto view = registry.view<Quality>();

    state.quality = Quality::Good;
    for (auto entity : view)
    {
        auto& quality = view.get<Quality>(entity);

        state.quality = minQuality(state.quality, quality);
    }

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


void updateEngineQuality(LevelData& state, std::chrono::duration<float> dt)
{
    entt::registry& registry = state.entities;
    auto view = registry.view<Engine, Durability, Quality>();

    for (auto entity : view)
    {
        auto& durability = view.get<Durability>(entity).durability;
        auto& quality = view.get<Quality>(entity);
        auto& engine = view.get<Engine>(entity);

        quality = computeQuality(durability, 0.05, 0.75);
        if (quality == Quality::Good)
        {
            engine.shake = 1.f;
        }
        else
        {
            engine.shake = std::max(0.f, engine.shake - dt / Constants::ENGINE_SHAKE_DURATION);
        }
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
    level.camera = Matrix::CreateTranslation(OScreenf / 2.f);

    auto& registry = level.entities;
    updateHoverStates(registry, dt, level.camera);
    updateHoverSounds(registry);
    updateGears(registry, dt);
    updateEngines(registry, dt);
    updateHamsters(registry, dt);
    updateDurability(level, dt);
    updateWear(level, dt);
    updateGearQuality(level, dt);
    updateEngineQuality(level, dt);
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
