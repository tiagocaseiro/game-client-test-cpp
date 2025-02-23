#include "TransformComponent.h"

const std::string& TransformComponent::ID()
{
    static const std::string id = "TransformComponent";
    return id;
}

ComponentShared TransformComponent::MakeComponent(GameObjectRef owner, King::Engine& engine,
                                                  const std::unordered_map<std::string, std::string>& parameters)
{
    glm::vec2 position = {};
    float rotation     = 0.0f;
    float scale        = 1.0f;

    if(auto it = parameters.find("posX"); it != std::end(parameters))
    {
        position.x = std::stof(it->second);
    }
    if(auto it = parameters.find("posY"); it != std::end(parameters))
    {
        position.y = std::stof(it->second);
    }

    if(auto it = parameters.find("rotation"); it != std::end(parameters))
    {
        rotation = std::stof(it->second);
    }

    if(auto it = parameters.find("scale"); it != std::end(parameters))
    {
        scale = std::stof(it->second);
    }

    return std::shared_ptr<Component>(new TransformComponent(owner, engine, position, rotation, scale));
}

TransformComponent::TransformComponent(GameObjectRef owner, King::Engine& engine, const glm::vec2& position,
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
