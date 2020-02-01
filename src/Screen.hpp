#pragma once
#include <memory>
#include <chrono>

struct Screen
{
    virtual ~Screen() = default;
    virtual std::unique_ptr<Screen> update(std::chrono::duration<float> dt) {
        return nullptr;
    }
    virtual void render() {}
};
