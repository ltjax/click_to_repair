#pragma once
#include <memory>
#include <chrono>
#include <onut/Music.h>
#include <functional>

struct Screen
{
    using Factory = std::function<std::unique_ptr<Screen>()>;
    virtual ~Screen() = default;
    virtual Factory update(std::chrono::duration<float> dt) {
        return {};
    }
    virtual void render() {}

    OMusicRef backgroundMusic;
};
