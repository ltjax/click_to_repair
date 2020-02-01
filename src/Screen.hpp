#pragma once
#include <memory>
#include <chrono>
#include <onut/Music.h>

struct Screen
{
    virtual ~Screen() = default;
    virtual std::unique_ptr<Screen> update(std::chrono::duration<float> dt) {
        return nullptr;
    }
    virtual void render() {}

    OMusicRef backgroundMusic;
};
