#pragma once
#include <onut/Vector4.h>

inline Rect enlargeRect(Rect input, Vector2 size)
{
    return Rect{input.x - size.x, input.y - size.y, input.z + size.x * 2.f, input.w + size.y * 2.f};
}

inline Rect shrinkRect(Rect input, Vector2 size)
{
    return enlargeRect(input, -size);
}

inline Vector2 inverseComponents(Vector2 p)
{
    return Vector2(1.f / p.x, 1.f / p.y);
}