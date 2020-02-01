#pragma once
#include "Screen.hpp"
#include "GameState.hpp"
#include <onut/Texture.h>
#include <onut/Anim.h>
#include <onut/Sound.h>

struct WinScreen : Screen
{
    WinScreen(Progress& progress, int finished_level);

    std::unique_ptr<Screen> update(std::chrono::duration<float> dt) override;
    void render() override;

private:
    Progress& progress_;
    int finished_level_;
    OTextureRef checkmark_;
    OAnimFloat anim_;
    OSoundInstanceRef sound_;
};
