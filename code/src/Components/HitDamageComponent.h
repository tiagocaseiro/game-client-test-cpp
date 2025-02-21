#pragma once

#include <king/CollisionWorld.h>

#include "glm/vec2.hpp"

#include "Component.h"

class CollisionBoxComponent;
class HealthComponent;
class ScoreComponent;

class HitDamageComponent : public Component, King::CollisionWorld::CollisionListener
{
public:
    static const std::string& ID();
    static ComponentShared MakeComponent(GameObjectRef owner, King::Engine& engine,
                                         const std::unordered_map<std::string, std::string>& parameters);

    ~HitDamageComponent();

    void Update() override;
    void OnCollision(int l, int r) override;

    // For debug purposes
    void DebugHit();

private:
    HitDamageComponent(GameObjectRef owner, King::Engine& engine);

    std::weak_ptr<CollisionBoxComponent> mCollisionBoxComponentRef;
    std::weak_ptr<HealthComponent> mHealthComponentRef;
    std::weak_ptr<ScoreComponent> mScoreComponentRef;
};