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
    float speed = 0.0f;
    if(auto it = parameters.find("speed"); it != std::end(parameters))
    {
        speed = std::stof(it->second);
    }

    return std::shared_ptr<Component>(new RigidBodyComponent(owner, engine, speed));
}

RigidBodyComponent::RigidBodyComponent(GameObjectRef owner, King::Engine& engine, const float speed)
    : Component(owner, engine), mRigidBody(nullptr), mSpeed(speed)
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
        collisionComponent->SetPosition(transformComponent->GetPosition());
        mRigidBody = mEngine.GetCollisionWorld().AddRigidBody(glm::normalize(glm::vec2(0, 1)) * mSpeed,
                                                              {collisionComponent->ColliderId()});

        return;
    }

    transformComponent->SetPosition(collisionComponent->ColliderPosition());
}