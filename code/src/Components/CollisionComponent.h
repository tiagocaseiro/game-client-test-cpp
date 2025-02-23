#pragma once

#include <optional>

#include "glm/vec2.hpp"

#include "Component.h"

class CollisionComponent : public Component
{
public:
    virtual ~CollisionComponent() = default;

    int ColliderId() const;

    virtual glm::vec2 ColliderPosition() const = 0;

    void SetPosition(const glm::vec2& position);

protected:
    CollisionComponent(GameObjectRef owner, King::Engine& engine, const glm::vec2& position, const unsigned int layer,
                       const unsigned int mask);
    virtual void InitCollisionId() = 0;

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

    ~CollisionBoxComponent();

private:
    CollisionBoxComponent(GameObjectRef owner, King::Engine& engine, const unsigned int layer, const unsigned int mask,
                          const glm::vec2& position, const glm::vec2& dimensions);

    void InitCollisionId() override;
    void OnCreate() override;

    glm::vec2 ColliderPosition() const override;

    glm::vec2 mDimensions;
};

class CollisionCircleComponent : public CollisionComponent
{
public:
    static const std::string& ID();
    static ComponentShared MakeComponent(GameObjectRef owner, King::Engine& engine,
                                         const std::unordered_map<std::string, std::string>& parameters);
    ~CollisionCircleComponent();

private:
    CollisionCircleComponent(GameObjectRef owner, King::Engine& engine, const unsigned int layer,
                             const unsigned int mask, const glm::vec2& position, const float radius);
    glm::vec2 ColliderPosition() const override;
    void InitCollisionId() override;
    void OnCreate() override;

    float mRadius;
};