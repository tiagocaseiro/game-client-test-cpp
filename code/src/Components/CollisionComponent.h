#pragma once

#include <optional>

#include "glm/vec2.hpp"

#include "Component.h"

namespace King
{
    struct BoxCollider;
    struct CircleCollider;
} // namespace King

class CircleCollider;

class CollisionComponent : public Component
{
public:
    virtual ~CollisionComponent() = default;

    int ColliderId() const;

    virtual glm::vec2 ColliderPosition() const = 0;

    virtual void SetPosition(const glm::vec2& position) = 0;

protected:
    CollisionComponent(GameObjectRef owner, King::Engine& engine, const glm::vec2& position, const unsigned int layer,
                       const unsigned int mask);

    int mColliderId;
    glm::vec2 mPosition;
    const unsigned int mLayer;
    const unsigned int mMask;
};

class CollisionBoxComponent final : public CollisionComponent
{
public:
    static const std::string& ID();
    static ComponentShared MakeComponent(GameObjectRef owner, King::Engine& engine,
                                         const std::unordered_map<std::string, std::string>& parameters);

    void SetDimensions(const glm::vec2& dimensions);
    void SetPosition(const glm::vec2& position) override;

    ~CollisionBoxComponent();

private:
    CollisionBoxComponent(GameObjectRef owner, King::Engine& engine, const unsigned int layer, const unsigned int mask,
                          const glm::vec2& position, const glm::vec2& dimensions);

    glm::vec2 ColliderPosition() const override;

    glm::vec2 mDimensions;
    King::BoxCollider* mBoxCollision;
};

class CollisionCircleComponent : public CollisionComponent
{
public:
    static const std::string& ID();
    static ComponentShared MakeComponent(GameObjectRef owner, King::Engine& engine,
                                         const std::unordered_map<std::string, std::string>& parameters);
    ~CollisionCircleComponent();

    void SetPosition(const glm::vec2& position) override;

    King::CircleCollider* Collider() const
    {
        return mCircleCollision;
    }

private:
    CollisionCircleComponent(GameObjectRef owner, King::Engine& engine, const unsigned int layer,
                             const unsigned int mask, const glm::vec2& position, const float radius);
    glm::vec2 ColliderPosition() const override;

    float mRadius;
    King::CircleCollider* mCircleCollision;
};