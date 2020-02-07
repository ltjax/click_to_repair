#pragma once
#include <onut/Vector4.h>
#include <onut/Color.h>
#include <onut/Input.h>
#include <onut/PrimitiveBatch.h>

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

inline Color get_color_focus(Rect const& rect)
{
  return rect.Contains(oInput->mousePosf) ? Color::White : Color{ 0.9f,0.9f,0.9f,1.f };
}

inline Rect get_fullscreen_rect()
{
  Vector2 size(96);
  return { 0, 0, size };
}

inline void drawCircle(Vector2 center, Color color, float radius, float stroke, int segments = 32)
{
    oPrimitiveBatch->begin(OPrimitiveTriangleList);

    for (auto i=0; i <= segments; ++i)
    {
        auto x1 = i*2*M_PI/segments;
        auto x2 = (i+1)*2*M_PI/segments;
        auto r1 = radius - stroke/2.f;
        auto r2 = radius + stroke/2.f;
        auto v1 = Vector2(cos(x1), sin(x1));
        auto v2 = Vector2(cos(x2), sin(x2));

        oPrimitiveBatch->draw(center+r1*v1,color);
        oPrimitiveBatch->draw(center+r2*v1,color);
        oPrimitiveBatch->draw(center+r1*v2,color);
        oPrimitiveBatch->draw(center+r1*v2,color);
        oPrimitiveBatch->draw(center+r2*v1,color);
        oPrimitiveBatch->draw(center+r2*v2,color);
    }

    oPrimitiveBatch->end();
}

inline void drawCircleFill(Vector2 center, Color color, float radius, int segments = 32)
{
    oPrimitiveBatch->begin(OPrimitiveTriangleList);

    for (auto i=0; i < segments; ++i)
    {
        auto x1 = i*2*M_PI/segments;
        auto x2 = (i+1)*2*M_PI/segments;
        auto v1 = radius * Vector2(cos(x1), sin(x1)) + center;
        auto v2 = radius * Vector2(cos(x2), sin(x2)) + center;

        oPrimitiveBatch->draw(center,color);
        oPrimitiveBatch->draw(v1,color);
        oPrimitiveBatch->draw(v2,color);
    }

    oPrimitiveBatch->end();
}
