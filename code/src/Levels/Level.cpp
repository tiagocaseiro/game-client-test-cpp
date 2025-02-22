// (C) king.com Ltd 2021
#include "Level.h"

#include "Components/CollisionComponent.h"
#include "Components/DamageOnCollisionComponent.h"
#include "Components/HealthComponent.h"
#include "Components/ScoreOnCollisionComponent.h"

Level::Level(King::Engine& engine, ScoreReportingFunction scoreReportingFunction)
    : mEngine(engine), mScoreReportingFunction(scoreReportingFunction)
{
}

void Level::AddGameObject(const GameObjectShared& gameObject)
{
    mGameObjects.emplace_back(gameObject);
}

void Level::MarkForDeath(const GameObjectShared& gameObject)
{
    mGameObjectsToDelete.emplace(gameObject);
}

void Level::Render()
{
    for(const GameObjectShared& gameObject : mGameObjects)
    {
        if(gameObject == nullptr)
        {
            continue;
        }

        for(const ComponentShared& component : gameObject->Components())
        {
            if(component)
            {
                component->Render();
            }
        }
    }
}

void Level::Update()
{
    // For safety, iterate through a copy of gameObjects because the original can be resized
    std::vector<GameObjectShared> gameObjects = mGameObjects;
    for(auto it = std::begin(mGameObjects); it != std::end(mGameObjects);)
    {
        std::shared_ptr<GameObject> gameObject = *it;
        if(gameObject == nullptr)
        {
            it = mGameObjects.erase(it);
            continue;
        }

        if(mGameObjectsToDelete.find(gameObject) != std::end(mGameObjectsToDelete))
        {
            it = mGameObjects.erase(it);
            continue;
        }

        for(const ComponentShared& component : gameObject->Components())
        {
            if(component)
            {
                component->Update();
            }
        }
        it++;
    }

    for(const GameObjectShared& gameObject : mGameObjectsToDelete)
    {
        for(const ComponentShared& component : gameObject->Components())
        {
            if(component)
            {
                component->OnDestroyed();
            }
        }
    }

    mGameObjectsToDelete.clear();
}

void Level::UpdateScore(int score) const
{
    mScoreReportingFunction(score);
}

int Level::NumBricksLeft() const
{
    return std::count_if(std::begin(mGameObjects), std::end(mGameObjects), [](const GameObjectRef& gameObjectShared) {
        return gameObjectShared.expired() == false && gameObjectShared.lock()->HasComponent<HealthComponent>();
    });
}

void Level::DebugDestroyAllBricks()
{
    mGameObjects.clear();
}

void Level::DebugDamageFirstBrick()
{
    if(mGameObjects.empty())
    {
        return;
    }

    for(GameObjectShared& brick : mGameObjects)
    {
        if(brick == nullptr)
        {
            continue;
        }

        std::shared_ptr<CollisionBoxComponent> collisionComponent = brick->FindComponent<CollisionBoxComponent>();
        if(collisionComponent == nullptr)
        {
            continue;
        }

        std::optional<int> colliderId = collisionComponent->ColliderId();
        if(colliderId.has_value() == false)
        {
            continue;
        }

        bool isToReturn = false;

        if(std::shared_ptr<DamageOnCollisionComponent> damageOnCollisionComponent =
               brick->FindComponent<DamageOnCollisionComponent>())
        {
            isToReturn = true;
            damageOnCollisionComponent->OnCollision(*colliderId, *colliderId);
        }

        if(std::shared_ptr<ScoreOnCollisionComponent> scoreOnCollisionComponent =
               brick->FindComponent<ScoreOnCollisionComponent>())
        {
            scoreOnCollisionComponent->OnCollision(*colliderId, *colliderId);
        }
        if(isToReturn)
        {
            return;
        }
    }
}
