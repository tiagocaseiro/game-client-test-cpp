#include "CollisionComponent.h"

#include <king/CollisionWorld.h>
#include <king/Engine.h>

const std::string& CollisionBoxComponent::ID()
{
    static const std::string id = "CollisionBoxComponent";
    return id;
}

ComponentShared CollisionBoxComponent::MakeComponent(GameObjectRef owner, King::Engine& engine,
                                                     const std::unordered_map<std::string, std::string>& parameters)
{
    glm::vec2 position;
    glm::vec2 dimensions;

    if(auto it = parameters.find("posX"); it != std::end(parameters))
    {
        position.x = std::stof(it->second);
    }
    if(auto it = parameters.find("posY"); it != std::end(parameters))
    {
        position.y = std::stof(it->second);
    }

    if(auto it = parameters.find("width"); it != std::end(parameters))
    {
        dimensions.x = std::stof(it->second);
    }

    if(auto it = parameters.find("height"); it != std::end(parameters))
    {
        dimensions.y = std::stof(it->second);
    }

    return std::shared_ptr<Component>(new CollisionBoxComponent(owner, engine, position, dimensions));
}

CollisionBoxComponent::CollisionBoxComponent(GameObjectRef owner, King::Engine& engine, const glm::vec2 position,
                                             const glm::vec2 dimensions)
    : Component(owner, engine), mPosition(position), mDimensions(dimensions)
{
    InitCollisionId();
}

void CollisionBoxComponent::UpdateData(const glm::vec2 position, const glm::vec2 dimensions)
{
    mPosition   = position;
    mDimensions = dimensions;

    InitCollisionId();
}

std::optional<int> CollisionBoxComponent::ColliderId() const
{
    return mColliderId;
}

CollisionBoxComponent::~CollisionBoxComponent()
{
    if(mColliderId)
    {
        mEngine.GetCollisionWorld().RemoveBoxCollider(*mColliderId);
    }
}

void CollisionBoxComponent::InitCollisionId()
{
    if(mColliderId)
    {
        mEngine.GetCollisionWorld().RemoveBoxCollider(*mColliderId);
        mColliderId = std::nullopt;
    }
    mColliderId = mEngine.GetCollisionWorld().AddBoxCollider(mPosition, mDimensions, 1 << 1, 0);
}

// const std::string& CollisionCircleComponent::ID()
//{
//     static const std::string id = "CollisionCircleComponent";
//     return id;
// }
//
// ComponentShared CollisionCircleComponent::MakeComponent(GameObjectRef owner, King::Engine& engine,
//                                                         const std::unordered_map<std::string, std::string>&
//                                                         parameters)
//{
//     return std::shared_ptr<Component>(new CollisionCircleComponent(owner, engine));
// }
//
// CollisionCircleComponent::CollisionCircleComponent(GameObjectRef owner, King::Engine& engine) : Component(owner,
// engine)
//{
// }
