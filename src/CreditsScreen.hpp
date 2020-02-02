#pragma once
#include "Screen.hpp"
#include "GameState.hpp"
#include <onut/Anim.h>
#include <onut/Texture.h>

struct CreditsScreen : Screen
{
    explicit CreditsScreen(std::shared_ptr<SharedState> sharedState);

    Factory update(std::chrono::duration<float> dt) override;
    void render() override;
private:
    std::shared_ptr<SharedState> sharedState_;
    OTextureRef checkmark_;
    OAnimFloat anim_;
    OAnimFloat zoom_;
};
