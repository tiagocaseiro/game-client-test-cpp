#pragma once

#include <king/CollisionWorld.h>

#include "glm/vec2.hpp"

#include "Component.h"

class CollisionComponent;

// Immediately marks GameObject for destruction down when the collider provider by one of the CollisionComponents
// detects a collision
class DestroyOnCollisionComponent final : public Component, King::CollisionWorld::CollisionListener
{
public:
    static const std::string& ID();
    static ComponentShared MakeComponent(GameObjectRef owner, King::Engine& engine,
                                         const std::unordered_map<std::string, std::string>& parameters);

    ~DestroyOnCollisionComponent();

    void OnCollision(int l, int r) override;

private:
    DestroyOnCollisionComponent(GameObjectRef owner, King::Engine& engine);

    void OnCreate() override;

    std::weak_ptr<CollisionComponent> mCollisionComponent;
};