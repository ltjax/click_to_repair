#pragma once
#include "Screen.hpp"
#include "GameState.hpp"
#include <onut/Texture.h>
#include <onut/Anim.h>
#include <onut/Sound.h>

struct WinScreen : Screen
{
    WinScreen(std::shared_ptr<SharedState> sharedState, int finished_level);

    Factory update(std::chrono::duration<float> dt) override;
    void render() override;

private:
    std::shared_ptr<SharedState> sharedState_;
    int finished_level_;
    OTextureRef checkmark_;
    OAnimFloat anim_;
    OSoundInstanceRef sound_;
};
