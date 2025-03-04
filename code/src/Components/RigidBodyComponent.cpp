#include "RigidBodyComponent.h"

#include <glm/glm.hpp>
#include <king/CollisionWorld.h>
#include <king/Engine.h>

#include "CollisionComponent.h"
#include "TransformComponent.h"

const std::string& RigidBodyComponent::ID()
{
    static const std::string id = "RigidBodyComponent";
    return id;
}

ComponentShared RigidBodyComponent::MakeComponent(GameObjectRef owner, King::Engine& engine,
                                                  const std::unordered_map<std::string, std::string>& parameters)
{
    glm::vec2 direction;
    if(auto it = parameters.find("directionX"); it != std::end(parameters))
    {
        direction.x = std::stof(it->second);
    }

    if(auto it = parameters.find("directionY"); it != std::end(parameters))
    {
        direction.y = std::stof(it->second);
    }

    float speed = 0.0f;
    if(auto it = parameters.find("speed"); it != std::end(parameters))
    {
        speed = std::stof(it->second);
    }

    return std::shared_ptr<Component>(new RigidBodyComponent(owner, engine, direction, speed));
}

RigidBodyComponent::RigidBodyComponent(GameObjectRef owner, King::Engine& engine, const glm::vec2& direction,
                                       const float speed)
    : Component(owner, engine), mRigidBody(nullptr), mDirection(glm::normalize(direction)), mSpeed(speed)
{
}

RigidBodyComponent::~RigidBodyComponent()
{
    mEngine.GetCollisionWorld().RemoveRigidBody(mRigidBody);
}

void RigidBodyComponent::OnCreate()
{

    mCollisionComponentRef = GetOwnerComponent<CollisionComponent>();
    mTransformComponentRef = GetOwnerComponent<TransformComponent>();
}

void RigidBodyComponent::Update()
{
    std::shared_ptr<CollisionComponent> collisionComponent = mCollisionComponentRef.lock();
    std::shared_ptr<TransformComponent> transformComponent = mTransformComponentRef.lock();

    if(collisionComponent == nullptr || transformComponent == nullptr)
    {
        return;
    }

    if(mRigidBody == nullptr)
    {

        // Collider's starting position match the transformer's
        collisionComponent->SetPosition(transformComponent->GetPosition());
        mRigidBody = mEngine.GetCollisionWorld().AddRigidBody(glm::normalize(mDirection) * mSpeed,
                                                              {collisionComponent->ColliderId()});
        return;
    }

    // Update transform with new position data from the collider that's being changed by the rigid body
    transformComponent->SetPosition(collisionComponent->ColliderPosition());
}