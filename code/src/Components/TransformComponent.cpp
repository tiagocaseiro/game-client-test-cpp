#include "TransformComponent.h"

const std::string& TransformComponent::ID()
{
    static const std::string id = "TransformComponent";
    return id;
}

ComponentShared TransformComponent::MakeComponent(GameObjectRef owner, King::Engine& engine,
                                                  const std::vector<std::string>& parameters)
{
    if(parameters.size() < 4)
    {
        return std::shared_ptr<Component>(new TransformComponent(owner, engine));
    }

    float x        = std::stof(parameters.at(0));
    float y        = std::stof(parameters.at(1));
    float rotation = std::stof(parameters.at(2));
    float scale    = std::stof(parameters.at(3));

    return std::shared_ptr<Component>(new TransformComponent(owner, engine, glm::vec2{x, y}, rotation, scale));
}

TransformComponent::TransformComponent(GameObjectRef owner, King::Engine& engine, const glm::vec2 position,
                                       const float rotation, const float scale)
    : Component(owner, engine), mPosition(position), mRotation(rotation), mScale(scale)
{
}

void TransformComponent::SetPosition(const glm::vec2 position)
{
    mPosition = position;
}

void TransformComponent::SetRotation(const float rotation)
{
    mRotation = rotation;
}

void TransformComponent::SetScale(const float scale)
{
    mScale = scale;
}

glm::vec2 TransformComponent::GetPosition() const
{
    return mPosition;
}

float TransformComponent::GetRotation() const
{
    return mRotation;
}

float TransformComponent::GetScale() const
{
    return mScale;
}
