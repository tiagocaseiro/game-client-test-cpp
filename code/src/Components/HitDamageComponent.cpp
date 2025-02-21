#include "HitDamageComponent.h"

#include <king/CollisionWorld.h>
#include <king/Engine.h>

#include "CollisionComponent.h"
#include "HealthComponent.h"

const std::string& HitDamageComponent::ID()
{
    static const std::string id = "HitDamageComponent";
    return id;
}

ComponentShared HitDamageComponent::MakeComponent(GameObjectRef owner, King::Engine& engine,
                                                  const std::unordered_map<std::string, std::string>& /*parameters*/)
{
    return std::shared_ptr<Component>(new HitDamageComponent(owner, engine));
}

HitDamageComponent::HitDamageComponent(GameObjectRef owner, King::Engine& engine) : Component(owner, engine)
{
    mEngine.GetCollisionWorld().AddCollisionListener(*this);
}

HitDamageComponent::~HitDamageComponent()
{
    mEngine.GetCollisionWorld().RemoveCollisionListener(*this);
}

void HitDamageComponent::Update()
{
    if(mCollisionBoxComponentRef.expired())
    {
        mCollisionBoxComponentRef = GetOwnerComponent<CollisionBoxComponent>();
    }

    if(mHealthComponentRef.expired())
    {
        mHealthComponentRef = GetOwnerComponent<HealthComponent>();
    }
}

void HitDamageComponent::OnCollision(int l, int r)
{
    if(mCollisionBoxComponentRef.expired() || mHealthComponentRef.expired())
    {
        return;
    }

    std::shared_ptr<CollisionBoxComponent> collisionBoxComponent = mCollisionBoxComponentRef.lock();
    std::optional<int> colliderId                                = collisionBoxComponent->ColliderId();

    if(colliderId.has_value() == false)
    {
        return;
    }

    if(l == *colliderId || r == *colliderId)
    {
        mHealthComponentRef.lock()->Decrement();
    }
}