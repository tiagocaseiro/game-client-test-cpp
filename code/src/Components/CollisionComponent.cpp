#include "CollisionComponent.h"

#include <king/CollisionWorld.h>
#include <king/Engine.h>

CollisionComponent::CollisionComponent(GameObjectRef owner, King::Engine& engine, const glm::vec2& position,
                                       const unsigned int layer, const unsigned int mask)
    : Component(owner, engine), mPosition(position), mLayer(layer), mMask(mask), mColliderId(-1)
{
}
int CollisionComponent::ColliderId() const
{
    return mColliderId;
}
void CollisionComponent::SetPosition(const glm::vec2& position)
{
    mPosition = position;
    InitCollisionId();
}

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
    unsigned int layer = 0;
    unsigned int mask  = 0;
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

    if(auto it = parameters.find("layer"); it != std::end(parameters))
    {
        layer = std::stoi(it->second);
    }

    if(auto it = parameters.find("mask"); it != std::end(parameters))
    {
        mask = std::stoi(it->second);
    }

    return std::shared_ptr<Component>(new CollisionBoxComponent(owner, engine, layer, mask, position, dimensions));
}

CollisionBoxComponent::CollisionBoxComponent(GameObjectRef owner, King::Engine& engine, const unsigned int layer,
                                             const unsigned int mask, const glm::vec2& position,
                                             const glm::vec2& dimensions)
    : CollisionComponent(owner, engine, position, layer, mask), mDimensions(dimensions)
{
    InitCollisionId();
}

void CollisionBoxComponent::SetDimensions(const glm::vec2& dimensions)
{
    mDimensions = dimensions;
    InitCollisionId();
}

CollisionBoxComponent::~CollisionBoxComponent()
{
    if(mColliderId)
    {
        mEngine.GetCollisionWorld().RemoveBoxCollider(mColliderId);
    }
}

void CollisionBoxComponent::InitCollisionId()
{
    if(mColliderId)
    {
        mEngine.GetCollisionWorld().RemoveBoxCollider(mColliderId);
    }
    mColliderId = mEngine.GetCollisionWorld().AddBoxCollider(mPosition, mDimensions, mLayer, mMask);
}

void CollisionBoxComponent::OnCreate()
{
    InitCollisionId();
}

glm::vec2 CollisionBoxComponent::ColliderPosition() const
{
    return {};
}

CollisionCircleComponent::~CollisionCircleComponent()
{
    mEngine.GetCollisionWorld().RemoveCircleCollider(mColliderId);
}

const std::string& CollisionCircleComponent::ID()
{
    static const std::string id = "CollisionCircleComponent";
    return id;
}

ComponentShared CollisionCircleComponent::MakeComponent(GameObjectRef owner, King::Engine& engine,
                                                        const std::unordered_map<std::string, std::string>& parameters)
{
    glm::vec2 position;
    float radius       = 0;
    unsigned int layer = 0;
    unsigned int mask  = 0;
    if(auto it = parameters.find("posX"); it != std::end(parameters))
    {
        position.x = std::stof(it->second);
    }

    if(auto it = parameters.find("posY"); it != std::end(parameters))
    {
        position.y = std::stof(it->second);
    }

    if(auto it = parameters.find("radius"); it != std::end(parameters))
    {
        radius = std::stof(it->second);
    }

    if(auto it = parameters.find("layer"); it != std::end(parameters))
    {
        layer = std::stoi(it->second);
    }

    if(auto it = parameters.find("mask"); it != std::end(parameters))
    {
        mask = std::stoi(it->second);
    }
    return std::shared_ptr<Component>(new CollisionCircleComponent(owner, engine, layer, mask, position, radius));
}

CollisionCircleComponent::CollisionCircleComponent(GameObjectRef owner, King::Engine& engine, const unsigned int layer,
                                                   const unsigned int mask, const glm::vec2& position,
                                                   const float radius)
    : CollisionComponent(owner, engine, position, layer, mask), mRadius(radius)
{
}

glm::vec2 CollisionCircleComponent::ColliderPosition() const
{
    if(auto collider = mEngine.GetCollisionWorld().GetCircleCollider(mColliderId))
    {
        return collider->mPosition;
    }
    return {};
}

void CollisionCircleComponent::InitCollisionId()
{
    if(mColliderId)
    {
        mEngine.GetCollisionWorld().RemoveCircleCollider(mColliderId);
    }
    mColliderId = mEngine.GetCollisionWorld().AddCircleCollider(mPosition, mRadius, mLayer, mMask);
}

void CollisionCircleComponent::OnCreate()

{
    InitCollisionId();
}
