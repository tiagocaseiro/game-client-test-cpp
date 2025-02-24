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
    : CollisionComponent(owner, engine, position, layer, mask), mDimensions(dimensions), mBoxCollision(nullptr)
{
    mColliderId   = mEngine.GetCollisionWorld().AddBoxCollider(mPosition, mDimensions, mLayer, mMask);
    mBoxCollision = mEngine.GetCollisionWorld().GetBoxCollider(mColliderId);
}

void CollisionBoxComponent::SetDimensions(const glm::vec2& dimensions)
{
    mDimensions = dimensions;
}

CollisionBoxComponent::~CollisionBoxComponent()
{
    if(mColliderId)
    {
        mEngine.GetCollisionWorld().RemoveBoxCollider(mColliderId);
    }
}

glm::vec2 CollisionBoxComponent::ColliderPosition() const
{
    if(mBoxCollision)
    {
        return mBoxCollision->mTopLeft;
    }
    return {};
}

void CollisionBoxComponent::SetPosition(const glm::vec2& position)
{
    if(mBoxCollision)
    {
        mBoxCollision->Set(position, mBoxCollision->mSize);
    }
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

CollisionCircleComponent::CollisionCircleComponent(GameObjectRef owner, King::Engine& engine, const unsigned int layer,
                                                   const unsigned int mask, const glm::vec2& position,
                                                   const float radius)
    : CollisionComponent(owner, engine, position, layer, mask), mRadius(radius), mCircleCollision(nullptr)
{
    mColliderId      = mEngine.GetCollisionWorld().AddCircleCollider(mPosition, mRadius, mLayer, mMask);
    mCircleCollision = mEngine.GetCollisionWorld().GetCircleCollider(mColliderId);
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

glm::vec2 CollisionCircleComponent::ColliderPosition() const
{
    if(mCircleCollision)
    {
        return mCircleCollision->mPosition;
    }
    return {};
}

void CollisionCircleComponent::SetPosition(const glm::vec2& position)
{
    if(mCircleCollision)
    {
        mCircleCollision->mPosition = position;
    }
}
