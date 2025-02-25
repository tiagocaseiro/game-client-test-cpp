#pragma once

#include <optional>

#include <king/CollisionWorld.h>

#include "Component.h"

class HealthComponent;
class CollisionComponent;

namespace King
{
    class Engine;
}

// Updates score when collider provider by one of the CollisionComponents detects a collision
class ScoreOnCollisionComponent final : public Component, King::CollisionWorld::CollisionListener
{

public:
    static const std::string& ID();
    static ComponentShared MakeComponent(GameObjectRef owner, King::Engine& engine,
                                         const std::unordered_map<std::string, std::string>& parameters);
    ~ScoreOnCollisionComponent();

    void OnCollision(int l, int r) override;

private:
    ScoreOnCollisionComponent(GameObjectRef owner, King::Engine& engine, const int score,
                              const std::string& targetColliderType);
    void OnCreate() override;

private:
    const int mScore;
    std::weak_ptr<CollisionComponent> mCollisionComponentRef;
    std::weak_ptr<HealthComponent> mHealthComponentRef;
    std::optional<int> mTargetColliderId;
};