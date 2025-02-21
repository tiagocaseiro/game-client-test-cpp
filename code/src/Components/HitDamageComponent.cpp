#include "HitDamageComponent.h"

#include <king/CollisionWorld.h>
#include <king/Engine.h>

#include "CollisionComponent.h"
#include "Game/GameObject.h"
#include "HealthComponent.h"
#include "Levels/Level.h"
#include "ScoreComponent.h"

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

    if(mScoreComponentRef.expired())
    {
        mScoreComponentRef = GetOwnerComponent<ScoreComponent>();
    }
}

void HitDamageComponent::OnCollision(int l, int r)
{
    if(mCollisionBoxComponentRef.expired())
    {
        return;
    }

    std::shared_ptr<CollisionBoxComponent> collisionBoxComponent = mCollisionBoxComponentRef.lock();

    std::optional<int> colliderId = collisionBoxComponent->ColliderId();

    if(colliderId && (l == *colliderId || r == *colliderId))
    {
        std::shared_ptr<HealthComponent> healthComponent = mHealthComponentRef.lock();

        if(healthComponent)
        {
            HandleCollisionOnMortal(*healthComponent);
            return;
        }

        HandleCollisionOnImmortal();
    }
}

void HitDamageComponent::HandleCollisionOnImmortal()
{
    if(mScoreComponentRef.expired() || mOwnerRef.expired())
    {
        return;
    }
    GameObjectShared owner = mOwnerRef.lock();

    owner->GameLevel().UpdateScore(ScoreComponent::DefaultScore());
}

void HitDamageComponent::HandleCollisionOnMortal(HealthComponent& healthComponent)
{
    if(healthComponent.IsAlive() == false)
    {
        return;
    }

    healthComponent.Decrement();

    if(mScoreComponentRef.expired() || mOwnerRef.expired())
    {
        return;
    }

    std::shared_ptr<ScoreComponent> scoreComponent = mScoreComponentRef.lock();

    GameObjectShared owner = mOwnerRef.lock();

    if(healthComponent.IsAlive())
    {
        owner->GameLevel().UpdateScore(ScoreComponent::DefaultScore());
    }
    else
    {
        owner->GameLevel().UpdateScore(scoreComponent->ScoreValue());
    }
}

void HitDamageComponent::DebugHit()
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
