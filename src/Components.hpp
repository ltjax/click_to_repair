#pragma once
#include <onut/Vector2.h>
#include <onut/Vector4.h>
#include <onut/Matrix.h>

struct Durability
{
    float durability = 1.f;
};

struct Machine
{
    Vector2 position;
    float size = 128.f;

    Rect getBoundingBox() const
    {
        auto halfSize = size * 0.5f;
        return Rect{ -halfSize + position.x, -halfSize + position.y, size, size };
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
};
