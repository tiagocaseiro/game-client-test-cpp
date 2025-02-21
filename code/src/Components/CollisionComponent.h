#pragma once

#include <optional>

#include "glm/vec2.hpp"

#include "Component.h"

class CollisionBoxComponent : public Component
{
public:
    static const std::string& ID();
    static ComponentShared MakeComponent(GameObjectRef owner, King::Engine& engine,
                                         const std::unordered_map<std::string, std::string>& parameters);

    ~CollisionBoxComponent();

    void UpdateData(const glm::vec2 position, const glm::vec2 dimensions);

private:
    CollisionBoxComponent(GameObjectRef owner, King::Engine& engine, const glm::vec2 position,
                          const glm::vec2 dimensions);

    void InitCollisionId();

    glm::vec2 mPosition;
    glm::vec2 mDimensions;
    std::optional<int> mCollisionId;
};

// class CollisionCircleComponent : public Component
//{
// public:
//     static const std::string& ID();
//     static ComponentShared MakeComponent(GameObjectRef owner, King::Engine& engine,
//                                          const std::unordered_map<std::string, std::string>& parameters);
//     ~CollisionCircleComponent();
//
// private:
//     CollisionCircleComponent(GameObjectRef owner, King::Engine& engine);
//     int mCollisionId;
// };