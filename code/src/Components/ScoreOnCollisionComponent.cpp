#include "ScoreOnCollisionComponent.h"

#include <sstream>

#include <king/Engine.h>

#include "CollisionComponent.h"
#include "Component.h"
#include "Game/GameObject.h"
#include "HealthComponent.h"
#include "Levels/Level.h"

ScoreOnCollisionComponent::ScoreOnCollisionComponent(GameObjectRef owner, King::Engine& engine, const int score)
    : Component(owner, engine), mScore(score)
{
    mEngine.GetCollisionWorld().AddCollisionListener(*this);
}

ScoreOnCollisionComponent::~ScoreOnCollisionComponent()
{
    mEngine.GetCollisionWorld().RemoveCollisionListener(*this);
}

void ScoreOnCollisionComponent::Update()
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

void ScoreOnCollisionComponent::OnCollision(int l, int r)
{
    if(mOwnerRef.expired() || mCollisionBoxComponentRef.expired())
    {
        return;
    }

    std::optional<int> colliderId = mCollisionBoxComponentRef.lock()->ColliderId();

    if((mHealthComponentRef.expired() || mHealthComponentRef.lock()->IsAlive()) && colliderId &&
       (l == *colliderId || r == *colliderId))
    {

        mOwnerRef.lock()->GameLevel().UpdateScore(mScore);
    }
}
const std::string& ScoreOnCollisionComponent::ID()
{
    static const std::string id = "ScoreOnCollisionComponent";
    return id;
}

ComponentShared ScoreOnCollisionComponent::MakeComponent(GameObjectRef owner, King::Engine& engine,
                                                         const std::unordered_map<std::string, std::string>& parameters)
{
    int score = 0;
    if(auto it = parameters.find("score"); it != std::end(parameters))
    {
        score = std::stoi(it->second);
    }

    return std::shared_ptr<Component>(new ScoreOnCollisionComponent(owner, engine, score));
}
