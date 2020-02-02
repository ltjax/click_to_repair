#include "Renderer.hpp"
#include "utils.hpp"
#include <onut/Texture.h>
#include "Components.hpp"
#include <onut/Renderer.h>
#include <onut/Input.h>
#include "Constants.hpp"
#include <onut/Model.h>

void renderMesh(OModel::Mesh* mesh, Matrix const& transform)
{
  oRenderer->renderStates.world = transform;
  oRenderer->renderStates.vertexBuffer = mesh->pVertexBuffer;
  oRenderer->renderStates.indexBuffer = mesh->pIndexBuffer;
  oRenderer->drawIndexed(mesh->elementCount);
}

void renderHamsters(LevelData const& level, Matrix centerScreen)
{
  auto model = OGetModel("hamster.obj");
  auto view = level.entities.view<Machine const, Hamster const, Hiccup const>();
  for (auto entity : view)
  {
        auto const& machine = view.get<Machine const>(entity);
        auto const& hamster = view.get<Hamster const>(entity);
        auto const& hiccup = view.get<Hiccup const>(entity);

        auto boundsX = std::abs(model->getBoundingBox()->x);
        auto scale = machine.size / (2.f * boundsX);
        auto scaleVec = Vector3{ scale, -scale, 1.f };
        auto jumpTransform = Matrix::CreateTranslation(0.f, 2.f * hiccup.jump_height, 0.f);

        auto hindLegAngle = std::sin(1000.f * OConvertToRadians(hamster.delta)) * 0.75f - 0.5f;
        auto frontLegAngle = std::sin(2000.f + 1000.f * OConvertToRadians(hamster.delta)) * 0.75f - 0.5f;
        auto hindLegTransform =
            Matrix::CreateRotationZ(hindLegAngle) *
            jumpTransform *
            Matrix::CreateTranslation(Vector2{ -0.65f, -1.3f }) *
            Matrix::CreateScale(scaleVec) *
            Matrix::CreateTranslation(machine.position) *
            centerScreen;
        renderMesh(model->getMesh(2), hindLegTransform);
    
        auto frontLegTransform =
            Matrix::CreateRotationZ(frontLegAngle) *
            jumpTransform *
            Matrix::CreateTranslation(Vector2{ 0.55f, -1.6f }) *
            Matrix::CreateScale(scaleVec) *
            Matrix::CreateTranslation(machine.position) *
            centerScreen;
        renderMesh(model->getMesh(0), frontLegTransform);

        auto hamsterTransform =
            jumpTransform *
            Matrix::CreateTranslation(Vector2{ 0.f, -1.3f }) *
            Matrix::CreateScale(scaleVec) *
            Matrix::CreateTranslation(machine.position) *
            centerScreen;
        renderMesh(model->getMesh(1), hamsterTransform);

        auto wheel_transform =
            Matrix::CreateRotationZ(-100.f * hamster.delta) *
            Matrix::CreateScale(scaleVec) *
            Matrix::CreateTranslation(machine.position) *
            centerScreen;
        renderMesh(model->getMesh(4), wheel_transform);

        auto stand_transform =
          Matrix::CreateScale(scaleVec) *
          Matrix::CreateTranslation(machine.position) *
          centerScreen;
        renderMesh(model->getMesh(3), stand_transform);
  }
}

void renderEngines(LevelData const& level, Matrix centerScreen)
{
  auto model = OGetModel("engine.obj");
  auto view = level.entities.view<Machine const, Engine const>();
  for (auto entity : view)
  {
    auto const& machine = view.get<Machine const>(entity);
    auto const& engine = view.get<Engine const>(entity);

    auto boundsX = std::abs(model->getBoundingBox()->x);
    auto scale = machine.size / (2.5f * boundsX);

    auto shakeFactor = engine.shake * 0.3 * level.animation_factor;
    auto shakeX = std::sin(rand()) * shakeFactor;
    auto shakeY = std::cos(rand()) * shakeFactor;
    auto shakeTranslation = Matrix::CreateTranslation(shakeX, shakeY, 0.f);
    auto cam_shaft_transform =
        shakeTranslation * 
        Matrix::CreateScale(scale) *
        Matrix::CreateTranslation(machine.position - Vector2{ 0.f, scale }) *
        centerScreen;
    renderMesh(model->getMesh(0), cam_shaft_transform);
  
    auto angle = engine.camShaftAngle * 8.f;
    auto rod_offset = Vector2{ std::cos(angle), std::sin(angle) - 2.f } * 0.5f;
    auto piston_offset = Vector2{ 0.f, 3.2f + rod_offset.y };

    auto rod_transform = 
        shakeTranslation *
        Matrix::CreateTranslation(rod_offset) * 
        Matrix::CreateScale(scale) * 
        Matrix::CreateTranslation(machine.position) *
        centerScreen;
    renderMesh(model->getMesh(1), rod_transform);// rod

    auto head_transform = Matrix::CreateScale(scale) *
        Matrix::CreateTranslation(machine.position - Vector2{ 0.f, scale + 25.f }) *
        centerScreen;
    renderMesh(model->getMesh(2), head_transform);

    auto piston_transform =
        shakeTranslation *
        Matrix::CreateTranslation(piston_offset) * 
        Matrix::CreateScale(scale) * 
        Matrix::CreateTranslation(machine.position) *
        centerScreen;
    renderMesh(model->getMesh(3), piston_transform); // render piston
  }
}

void renderGears(LevelData const& level, Matrix centerScreen)
{
    auto model = OGetModel("gears.obj");
    auto view = level.entities.view<Machine const, Gear const>();
    for (auto entity : view)
    {
        auto const& machine = view.get<Machine const>(entity);
        auto const& gear = view.get<Gear const>(entity);
        
        float const wobbleAngle = 0.2f * std::sin(8.f * gear.delta);
        auto boundsX = std::abs(model->getBoundingBox()->x);
        auto scale = machine.size / (2.f * boundsX);
        auto small_wheel_transform =
            Matrix::CreateScale(scale) *
            gear.rotation *
            Matrix::CreateTranslation(machine.position - Vector2{ 1.2f, -0.5f } * scale) *
            centerScreen;
        
        auto large_wheel_transform =
            Matrix::CreateScale(scale) *
            gear.rotation.Invert() *
            Matrix::CreateTranslation(machine.position - Vector2{-2.f, -0.5f} * scale) *
            centerScreen;

        renderMesh(model->getMesh(0), small_wheel_transform);
        renderMesh(model->getMesh(1), large_wheel_transform);
    }
}

void renderFluxCapacitors(LevelData const& level, Matrix centerScreen)
{
    auto model = OGetModel("flux_capacitor.obj");
    auto view = level.entities.view<Machine const, FluxCapacitor const>();
    for (auto entity : view)
    {
        auto const& machine = view.get<Machine const>(entity);
        auto boundsX = std::abs(model->getBoundingBox()->x);
        auto scale = machine.size / (2.f * boundsX);
        auto flux_box_transform =
            Matrix::CreateScale(scale) * 
            Matrix::CreateTranslation(machine.position - Vector2{ 0.f, -1.f * scale }) *
            centerScreen;
        renderMesh(model->getMesh(0), flux_box_transform);
    }
}

Color colorForQuality(Quality quality)
{
    auto color = OColorRGB(255, 255, 255);
    if (quality == Quality::Worst)
    {
        color = OColorRGB(220, 20, 20);
    }
    else if (quality == Quality::Medium)
    {
        color = OColorRGB(220, 220, 20);
    }
    return color;
}


void renderBar(OSpriteBatchRef spriteBatch, Rect rectangle, float fullness, Color color)
{
    auto texture = OGetTexture("bar.png");
    auto contentRect = shrinkRect(rectangle, Vector2{ 2 });
    contentRect.z *= fullness;
    oSpriteBatch->drawRect(nullptr, contentRect, color);
    oSpriteBatch->drawRectScaled9(texture, rectangle, Vector4{ 4.f });
}

void renderMachineFrames(entt::registry const& registry, Matrix const& camera)
{
    auto frame = OGetTexture("frame.png");
    auto frame_size = frame->getSizef();
    auto view = registry.view<Machine const, QualityStatus const>();

    for (auto entity : view)
    {
        auto const& machine = view.get<Machine const>(entity);
        auto const& quality = view.get<QualityStatus const>(entity);

        auto rotation = 0.f;
        if (auto hover = registry.try_get<HoverState const>(entity); hover && hover->containsMouse)
          rotation = 3.f * std::sin(5.f * hover->timeIn.count()) + 2.f;

        Color color = Color::White; 

        if (machine.warningDuration > std::chrono::duration<float>::zero()) 
        {
            color = colorForQuality(quality.current);

        }
        oSpriteBatch->drawSprite(frame, Vector2::Transform(machine.position, camera), color , rotation);

    }
}

void renderDurabilityBar(entt::registry const& registry, Matrix const& camera)
{
    auto view = registry.view<Machine const, Durability const, QualityStatus const>();

    for (auto entity : view)
    {
        auto const& machine = view.get<Machine const>(entity);
        auto durability = view.get<Durability const>(entity).durability;
        auto quality = view.get<QualityStatus const>(entity).current;

        auto p = Vector2::Transform(machine.position, camera);
        auto size = machine.size;
        auto halfSize = size * 0.5f;
        auto barHeight = 20.f;
        auto backgroundRect = Rect{ -halfSize + p.x, -halfSize + p.y - barHeight, size, barHeight };
        renderBar(oSpriteBatch, backgroundRect, durability, colorForQuality(quality));
    }
}

bool isHoveringAnyMachine(LevelData const& state)
{
  auto view = state.entities.view<Machine const, HoverState const>();
  for (auto entity : view)
  {
    auto const& hoverState = view.get<HoverState const>(entity);
    if (hoverState.containsMouse)
      return true;
  }
  return false;
}

void renderRepairiumBar(LevelData const& state)
{
    auto barHeight = 45.f;
    auto screenSize = OScreenf;
    auto rect = Rect{ 0.f, screenSize.y - barHeight, screenSize.x, barHeight };
    rect = shrinkRect(rect, Vector2{ 8.f });
    auto d = state.is_repairing || !isHoveringAnyMachine(state);
    if (state.is_repairing)
    {
      renderBar(oSpriteBatch, rect, state.repairium, Color::fromHexRGB(0xff0000));
      renderBar(oSpriteBatch, rect, std::max(0.f, state.repairium - Constants::REPAIR_RESOURCE_DRAIN_PER_SECOND * 0.5f), Color::White);
    }
    else if ( !isHoveringAnyMachine(state))
    {
      renderBar(oSpriteBatch, rect, state.repairium, Color::White);
    }
    else
    {
      renderBar(oSpriteBatch, rect, state.repairium, Color::fromHexRGB(0xff0000));
      renderBar(oSpriteBatch, rect, std::max(0.f, state.repairium - Constants::REPAIR_UP_FRONT_COST - Constants::REPAIR_RESOURCE_DRAIN_PER_SECOND * 0.5f), Color::White);
    }
}

void renderQualityLights(LevelData const& state)
{
    auto screenSize = OScreenf;
    auto texture = OGetTexture("white_dot.png");
    auto circle = OGetTexture("quality_circle.png");
    auto padding = 32.f;

    auto offset = Vector2{ padding, 0.f };
    Vector2 positionCenter{ screenSize.x / 2.f, texture->getSizef().y / 2.f + padding };
    Vector2 positionLeft = positionCenter - offset;
    Vector2 positionRight = positionCenter + offset;

    auto quality = state.quality.current;

    struct { Vector2 position; Color color; } lights[]{
        {positionLeft, quality == Quality::Worst ? OColorRGB(255, 0, 0) : OColorRGB(75, 0, 0)},
        {positionCenter, quality == Quality::Medium ? OColorRGB(255, 255, 0) : OColorRGB(75, 75, 0)},
        {positionRight, quality == Quality::Good ? OColorRGB(0, 255, 0) : OColorRGB(0, 75, 0)}
    };

    for (auto const& each : lights)
    {
        auto transform = Matrix::CreateTranslation(each.position);
        oSpriteBatch->drawSprite(texture, transform, each.color);
        oSpriteBatch->drawSprite(circle, transform );
    }
}

void renderCursor(LevelData const& state)
{
    oInput->setMouseVisible(!state.is_repairing);

    if (!state.is_repairing)
        return;

    auto icon_wrench = OGetTexture("wrench.png");
    auto textureSize = icon_wrench->getSizef();

    const float targetSize = 96;
    static float rot_time;
    auto s = std::sin(state.repair_time * 7.f);
    float rotation = (s < 0 ? 1.f : -1.f) * (s * s) * 25.f;
    oSpriteBatch->drawSprite(icon_wrench,
        oInput->mousePosf,
        Color::White, rotation, targetSize / textureSize.x,
        Vector2(0.1f, 0.1f));
}


void Renderer::run()
{
    oRenderer->setupFor3D({ 0.f, 1.f, 0.f }, Vector3::Zero, Vector3::Up, 90.f);
    oRenderer->set2DCamera(Vector2::Zero);
    oRenderer->clear(Constants::BackgroundColor());
    oRenderer->setAmbient(Color::White);
    oRenderer->clearDepth();

    auto& registry = level.entities;
    oRenderer->renderStates.blendMode = OBlendOpaque;
    oRenderer->renderStates.backFaceCull = false;
    renderGears(level, level.camera);
    renderEngines(level, level.camera);
    renderHamsters(level, level.camera);
    renderFluxCapacitors(level, level.camera);

    oRenderer->renderStates.blendMode = OBlendPreMultiplied;
    oSpriteBatch->begin();
    renderMachineFrames(registry, level.camera);
    renderDurabilityBar(registry, level.camera);
    renderRepairiumBar(level);
    renderQualityLights(level);
    oRenderer->renderStates.blendMode = OBlendAlpha;
    renderCursor(level); // should be called last, so it is rendered on top

    oSpriteBatch->end();
}
