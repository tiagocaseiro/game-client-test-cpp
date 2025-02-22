#pragma once

#include "glm/vec2.hpp"

#include "Component.h"

class TransformComponent final : public Component
{
public:
    static const std::string& ID();
    static ComponentShared MakeComponent(GameObjectRef owner, King::Engine& engine,
                                         const std::unordered_map<std::string, std::string>& parameters);
    void SetPosition(const glm::vec2 position);
    void SetRotation(const float rotation);
    void SetScale(const float scale);

    glm::vec2 GetPosition() const;
    float GetRotation() const;
    float GetScale() const;

private:
    TransformComponent(GameObjectRef owner, King::Engine& engine, const glm::vec2 position = {},
                       const float rotation = 0.0f, const float scale = 1.0f);

    glm::vec2 mPosition;
    float mRotation;
    float mScale;
};