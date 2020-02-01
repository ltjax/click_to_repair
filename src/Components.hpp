#pragma once
#include <onut/Vector2.h>
#include <onut/Vector4.h>
#include <onut/Matrix.h>
#include <onut/Sound.h>

struct Durability
{
    float durability = 1.f;
};

struct Machine
{
    Vector2 position;
    float size = 128.f;

    Rect getBoundingBox(Matrix const& camera) const
    {
        auto p = Vector2::Transform(position, camera);
        auto halfSize = size * 0.5f;
        return Rect{ -halfSize + p.x, -halfSize + p.y, size, size };
    }

};

struct Gear
{
    Matrix rotation;
    float delta = 0.f;
};

struct Engine
{
    float cam_shaft_angle = 0.f;
    float shake = 1.f;
};

struct HoverState
{
    bool containsMouse = false;
    std::chrono::duration<float> timeIn = std::chrono::duration<float>::zero();
    std::chrono::duration<float> timeOut = std::chrono::duration<float>::zero();
};

struct HoverSound
{
    OSoundInstanceRef background;
};
