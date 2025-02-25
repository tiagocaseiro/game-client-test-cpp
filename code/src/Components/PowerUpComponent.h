#pragma once

#include <king/CollisionWorld.h>

#include "glm/vec2.hpp"

#include "Component.h"

class CollisionComponent;
class Paddle;

// Is able to temporarily change the Paddle's size and sprite when the collider provider by one of the
// CollisionComponents detects a collision
class PaddleResizePowerUpComponent final : public Component, King::CollisionWorld::CollisionListener

{
public:
    static const std::string& ID();
    static ComponentShared MakeComponent(GameObjectRef owner, King::Engine& engine,
                                         const std::unordered_map<std::string, std::string>& parameters);

    ~PaddleResizePowerUpComponent();

    void OnCollision(int l, int r) override;

private:
    PaddleResizePowerUpComponent(GameObjectRef owner, King::Engine& engine, const std::optional<float> timer,
                                 const std::optional<int> width, const std::optional<int> height,
                                 const std::optional<int> newTextureHandle);

    void OnCreate() override;

    std::weak_ptr<CollisionComponent> mCollisionComponentRef;

    const std::optional<float> mTimer;
    const std::optional<int> mWidth;
    const std::optional<int> mHeight;
    const std::optional<int> mNewTextureHandle;
};