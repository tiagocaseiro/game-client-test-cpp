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
    mCollisionBoxComponentRef = GetOwnerComponent<CollisionBoxComponent>();
    mHealthComponentRef       = GetOwnerComponent<HealthComponent>();
}

void DamageOnCollisionComponent::OnCollision(int l, int r)
{
    if(mCollisionBoxComponentRef.expired() || mHealthComponentRef.expired())
    {
        return;
    }

    std::shared_ptr<CollisionBoxComponent> collisionBoxComponent = mCollisionBoxComponentRef.lock();

    std::optional<int> colliderId = collisionBoxComponent->ColliderId();

    if(colliderId && (l == *colliderId || r == *colliderId))
    {
        std::shared_ptr<HealthComponent> healthComponent = mHealthComponentRef.lock();
        healthComponent->Decrement();
    }
}

void DamageOnCollisionComponent::DebugHit()
{
    if(mCollisionBoxComponentRef.expired())
    {
        return;
    }

    std::shared_ptr<CollisionBoxComponent> collisionBoxComponent = mCollisionBoxComponentRef.lock();

    std::optional<int> colliderId = collisionBoxComponent->ColliderId();

    if(colliderId.has_value() == false)
    {
        return;
    }

    OnCollision(*colliderId, 0);
}
