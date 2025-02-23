#include "DamageOnCollisionComponent.h"

#include <king/Engine.h>

#include "CollisionComponent.h"
#include "Game/GameObject.h"
#include "HealthComponent.h"
#include "Levels/Level.h"

const std::string& DamageOnCollisionComponent::ID()
{
    static const std::string id = "DamageOnCollisionComponent";
    return id;
}

ComponentShared DamageOnCollisionComponent::MakeComponent(
    GameObjectRef owner, King::Engine& engine, const std::unordered_map<std::string, std::string>& /*parameters*/)
{
    return std::shared_ptr<Component>(new DamageOnCollisionComponent(owner, engine));
}

DamageOnCollisionComponent::DamageOnCollisionComponent(GameObjectRef owner, King::Engine& engine)
    : Component(owner, engine)
{
    mEngine.GetCollisionWorld().AddCollisionListener(*this);
}

DamageOnCollisionComponent::~DamageOnCollisionComponent()
{
    mEngine.GetCollisionWorld().RemoveCollisionListener(*this);
}

void DamageOnCollisionComponent::OnCreate()
{
    mCollisionComponent = GetOwnerComponent<CollisionComponent>();
    mHealthComponentRef = GetOwnerComponent<HealthComponent>();
}

void DamageOnCollisionComponent::OnCollision(int l, int r)
{
    std::shared_ptr<CollisionComponent> collisionComponent = mCollisionComponent.lock();
    std::shared_ptr<HealthComponent> healthComponent       = mHealthComponentRef.lock();

    if(collisionComponent == nullptr || healthComponent == nullptr)
    {
        return;
    }

    const int colliderId = collisionComponent->ColliderId();

    if(l == colliderId || r == colliderId)
    {
        healthComponent->Decrement();
    }
}

void DamageOnCollisionComponent::DebugHit()
{
    if(mCollisionComponent.expired())
    {
        return;
    }

    std::shared_ptr<CollisionComponent> collisionComponent = mCollisionComponent.lock();

    std::optional<int> colliderId = collisionComponent->ColliderId();

    if(colliderId.has_value() == false)
    {
        return;
    }

    OnCollision(*colliderId, 0);
}
