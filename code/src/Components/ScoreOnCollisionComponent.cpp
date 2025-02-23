#include "ScoreOnCollisionComponent.h"

#include <sstream>

#include <king/Engine.h>

#include "CollisionComponent.h"
#include "Component.h"
#include "Game/GameObject.h"
#include "GameStates/GamePlayState.h"
#include "HealthComponent.h"

ScoreOnCollisionComponent::ScoreOnCollisionComponent(GameObjectRef owner, King::Engine& engine, const int score)
    : Component(owner, engine), mScore(score)
{
    mEngine.GetCollisionWorld().AddCollisionListener(*this);
}

ScoreOnCollisionComponent::~ScoreOnCollisionComponent()
{
    mEngine.GetCollisionWorld().RemoveCollisionListener(*this);
}

void ScoreOnCollisionComponent::OnCreate()
{
    mCollisionComponentRef = GetOwnerComponent<CollisionComponent>();
    mHealthComponentRef    = GetOwnerComponent<HealthComponent>();
}

void ScoreOnCollisionComponent::OnCollision(int l, int r)
{
    if(mOwnerRef.expired() || mCollisionComponentRef.expired())
    {
        return;
    }

    int colliderId = mCollisionComponentRef.lock()->ColliderId();

    if((mHealthComponentRef.expired() || mHealthComponentRef.lock()->IsAlive()) && (l == colliderId || r == colliderId))
    {

        mOwnerRef.lock()->GameState().UpdateScore(mScore);
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
