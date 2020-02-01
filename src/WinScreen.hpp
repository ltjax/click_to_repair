#pragma once
#include "Screen.hpp"
#include <onut/Texture.h>

struct WinScreen : Screen
{
    WinScreen();

    std::unique_ptr<Screen> update(std::chrono::duration<float> dt) override;
    void render() override;

private:
    OTextureRef checkmark_;
};
