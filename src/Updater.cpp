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

void updateHamsterHiccups(LevelData& state, std::chrono::duration<float> dt)
{
    using D = std::uniform_real_distribution<float>;

    auto& registry = state.entities;
    auto hiccupDurabilityLoss = state.quality.current != Quality::Worst ? D(0.07f, 0.085f) : D(0.15f, 0.2f);
    D hiccupPause(2.0f, 2.4f);
    auto view = registry.view<Hamster>();
    for (auto entity : view)
    {
        auto& hamster = view.get<Hamster>(entity);
        if (hamster.nextHiccup <= std::chrono::duration<float>{0})
        {
            hamster.decay += hiccupDurabilityLoss(state.rng);
            hamster.nextHiccup = std::chrono::duration<float>(hiccupPause(state.rng));
            registry.assign<Hiccup>(entity, "hiccup.wav");
        }
        else
        {
            hamster.nextHiccup -= dt;
        }
    }
}

void updateHiccupEffects(entt::registry& registry)
{
    auto view = registry.view<Hiccup>();
    for (auto entity : view)
    {
        auto& hiccup = view.get<Hiccup>(entity);
        OPlaySound(hiccup.sfxFilename);
        registry.remove<Hiccup>(entity);
    }
}

void updateRepair(LevelData& state, std::chrono::duration<float> dt)
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

    if (!was_repairing && state.is_repairing)
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
    auto view = registry.view<Durability, Engine>();
    for (auto entity : view)
    {
        auto& durability = view.get<Durability>(entity).durability;

        auto wear = view.get<Engine>(entity).shake > 0.f ?
            Constants::ENGINE_WEAR_PER_SECOND_SHAKING :
            Constants::ENGINE_WEAR_PER_SECOND;
        durability = std::max(0.f, durability - delta.count() * wear);
    }
}

void updateHamsterWear(LevelData& state, std::chrono::duration<float> delta)
{
    entt::registry& registry = state.entities;
    auto view = registry.view<Durability, Hamster>();
    for (auto entity : view)
    {
        auto& durability = view.get<Durability>(entity).durability;
        auto& hamster = view.get<Hamster>(entity);

        if (hamster.decay <= 0.f)
            continue;

        float decayThisFrame = std::min(hamster.decay, delta.count() * 0.4f);
        hamster.decay -= decayThisFrame;
        durability = std::max(0.f, durability - decayThisFrame);
    }
}

void updateWear(LevelData& state, std::chrono::duration<float> dt)
{
    // Hamsters always wear down!
    updateHamsterWear(state, dt);

    // Everything else only does when the machine is running
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
                auto newVolume = std::clamp(hoverState.timeIn.count(), 0.f, 0.7f);
                hoverSound.background->setVolume(newVolume);
            }
        }
        if (!hoverState.containsMouse && hoverSound.background->isPlaying())
        {
            auto newVolume = 0.7f - std::clamp(hoverState.timeOut.count(), 0.f, 0.7f);
            hoverSound.background->setVolume(newVolume);
            if (hoverState.timeOut.count() > 0.7f)
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


void updateGlobalQuality(LevelData& state, std::chrono::duration<float> dt)
{
    entt::registry& registry = state.entities;
    auto view = registry.view<QualityStatus>();

    state.quality.previous = state.quality.current;
    state.quality.current = Quality::Good;
    for (auto entity : view)
    {
        auto& quality = view.get<QualityStatus>(entity).current;

        state.quality.current = std::min(state.quality.current, quality);
    }
}

void updateGlobalQualitySound(LevelData& state, std::chrono::duration<float> dt)
{
    entt::registry& registry = state.entities;
    auto view = registry.view<GlobalQualitySound>();

    for (auto entity :view)
    {
        auto sound = view.get<GlobalQualitySound>(entity);

        if (state.quality.current > state.quality.previous)
        {
            sound.positiveNotification->play();
        }

        if (state.quality.current < state.quality.previous)
        {
            sound.negativeNotification->play();
        }
    }

}

void updateGearQuality(LevelData& state, std::chrono::duration<float> dt)
{
    entt::registry& registry = state.entities;
    auto view = registry.view<Gear, Durability, QualityStatus>();

    for (auto entity : view)
    {
        auto& durability = view.get<Durability>(entity).durability;
        auto& quality = view.get<QualityStatus>(entity);

        quality.previous = quality.current;
        quality.current = computeQuality(durability, 0.1, 0.5);
    }
}


void updateEngineQuality(LevelData& state, std::chrono::duration<float> dt)
{
    entt::registry& registry = state.entities;
    auto view = registry.view<Engine, Durability, QualityStatus>();

    for (auto entity : view)
    {
        auto& durability = view.get<Durability>(entity).durability;
        auto& quality = view.get<QualityStatus>(entity);
        auto& engine = view.get<Engine>(entity);

        quality.previous = quality.current;
        quality.current = computeQuality(durability, 0.05, 0.75);
        if (quality.current == Quality::Good)
        {
            engine.shake = 1.f;
        }
        else
        {
            engine.shake = std::max(0.f, engine.shake - dt / Constants::ENGINE_SHAKE_DURATION);
        }
    }
}

void updateHamsterQuality(LevelData& state, std::chrono::duration<float> dt)
{
    entt::registry& registry = state.entities;
    auto view = registry.view<Hamster, Durability, QualityStatus>();

    for (auto entity : view)
    {
        auto& durability = view.get<Durability>(entity).durability;
        auto& quality = view.get<QualityStatus>(entity);

        quality.previous = quality.current;
        quality.current = computeQuality(durability, 0.05, 0.15);
    }
}


void updateRepairTime(LevelData& state, std::chrono::duration<float> dt)
{
    if (state.is_repairing)
    {
        auto previous = state.repair_time;
        state.repair_time += dt.count();

        auto sound_period = 0.5f;
        if (std::ceil(previous / sound_period) != std::ceil(state.repair_time / sound_period))
        {
            std::uniform_int_distribution<int> SoundIndex(1, 7);
            OPlaySound("wrench_" + std::to_string(SoundIndex(state.rng)) + ".wav", 1.f, 0.f, 1.f);
        }

        return; // No new repairium if we repair
    }
    else
    {
        state.repair_time = 0.f;
    }
}

void updateQualityTransitions(LevelData& state, std::chrono::duration<float> dt)
{
    entt::registry& registry = state.entities;
    auto view = registry.view<Engine, Durability, QualityStatus>();

    for (auto entity : view)
    {
        auto& quality = view.get<QualityStatus>(entity);


        if (quality.current > quality.previous) 
        {

        } 

        if (quality.current < quality.previous) 
        {

        }
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

    if (state.quality.current == Quality::Worst)
    {
        // machine is stopped
        return;
    }

    auto reparium_per_second = ((state.quality.current == Quality::Good) ? 0.1f : 0.05f) * state.reparium_multiplier;
    state.repairium += reparium_per_second * dt.count();
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
    updateHamsterHiccups(level, dt);
    updateHiccupEffects(registry);
    updateRepair(level, dt);
    updateWear(level, dt);
    updateGearQuality(level, dt);
    updateEngineQuality(level, dt);
    updateHamsterQuality(level, dt);
    updateGlobalQuality(level, dt);
    updateGlobalQualitySound(level, dt);
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
