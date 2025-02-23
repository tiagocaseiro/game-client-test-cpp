#include "DestroyOnCollisionComponent.h"

#include <king/Engine.h>

#include "CollisionComponent.h"
#include "Game/GameObject.h"
#include "GameStates/GamePlayState.h"
#include "HealthComponent.h"

const std::string& DestroyOnCollisionComponent::ID()
{
    static const std::string id = "DestroyOnCollisionComponent";
    return id;
}

ComponentShared DestroyOnCollisionComponent::MakeComponent(
    GameObjectRef owner, King::Engine& engine, const std::unordered_map<std::string, std::string>& /*parameters*/)
{
    return std::shared_ptr<Component>(new DestroyOnCollisionComponent(owner, engine));
}

DestroyOnCollisionComponent::DestroyOnCollisionComponent(GameObjectRef owner, King::Engine& engine)
    : Component(owner, engine)
{
    mEngine.GetCollisionWorld().AddCollisionListener(*this);
}

DestroyOnCollisionComponent::~DestroyOnCollisionComponent()
{
    mEngine.GetCollisionWorld().RemoveCollisionListener(*this);
}

void DestroyOnCollisionComponent::OnCreate()
{
    mCollisionComponent = GetOwnerComponent<CollisionComponent>();
}

void DestroyOnCollisionComponent::OnCollision(int l, int r)
{
    std::shared_ptr<CollisionComponent> collisionComponent = mCollisionComponent.lock();

    if(collisionComponent == nullptr || mOwnerRef.expired())
    {
        return;
    }

    const int colliderId = collisionComponent->ColliderId();

    if(l == colliderId || r == colliderId)
    {
        mOwnerRef.lock()->MarkForDeath();
    }
}
