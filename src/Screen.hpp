#pragma once
#include <memory>
#include <chrono>
#include <functional>

struct Screen
{
    using ScreenFactory = std::function<std::unique_ptr<Screen>()>;
    virtual ~Screen() = default;
    virtual ScreenFactory update(std::chrono::duration<float> dt) {
        return {};
    }
    virtual void render() {}
};
