#pragma once

#include <variant>

#include "glm/vec2.hpp"

#include "Component.h"

class CollisionComponent;
class TransformComponent;

namespace King
{
    struct RigidBody;
}

class RigidBodyComponent final : public Component
{
public:
    static const std::string& ID();
    static ComponentShared MakeComponent(GameObjectRef owner, King::Engine& engine,
                                         const std::unordered_map<std::string, std::string>& parameters);

    ~RigidBodyComponent();

private:
    RigidBodyComponent(GameObjectRef owner, King::Engine& engine, const float speed);
    void OnCreate() override;
    void Update() override;

    std::weak_ptr<CollisionComponent> mCollisionComponentRef;
    std::weak_ptr<TransformComponent> mTransformComponentRef;
    King::RigidBody* mRigidBody;
    const float mSpeed;
};