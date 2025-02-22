// (C) king.com Ltd 2021
#include "Level.h"

#include "Components/CollisionComponent.h"
#include "Components/DamageOnCollisionComponent.h"
#include "Components/HealthComponent.h"
#include "Components/ScoreOnCollisionComponent.h"

Level::Level(King::Engine& engine, ScoreReportingFunction scoreReportingFunction)
    : mEngine(engine), mScoreReportingFunction(scoreReportingFunction)
{
    mEngine.GetCollisionWorld().AddCollisionListener(*this);
}

Level::~Level()
{
    mEngine.GetCollisionWorld().RemoveCollisionListener(*this);
}

void Level::Reset()
{
    // mNumBricksLeft = 0;
}

// void Level::AddBrick(const glm::vec2& /*position*/, Brick::BrickType /*type*/, int /*hitPoints*/)
//{
//  Brick brick(mEngine, position, hitPoints, type);
//  int brickId = mEngine.GetCollisionWorld().AddBoxCollider(position, glm::vec2(kBrickWidth, kBrickHeight), 1 << 1,
//  0);

// mBricks.emplace(std::pair(brickId, std::move(brick)));

// if(type != Brick::BRICK_SOLID)
//{
//     mNumBricksLeft++;
// }
//}

void Level::AddBrick(const GameObjectShared& brick)
{
    mBricks.emplace_back(brick);
}

void Level::MarkForDeath(const GameObjectShared& gameObject)
{
    mGameObjectsToDelete.emplace(gameObject);
}

void Level::Render()
{
    for(const GameObjectShared& brick : mBricks)
    {
        if(brick == nullptr)
        {
            continue;
        }

        for(const ComponentShared& component : brick->Components())
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
    for(auto it = std::begin(mBricks); it != std::end(mBricks);)
    {
        std::shared_ptr<GameObject> brick = *it;
        if(brick == nullptr)
        {
            it = mBricks.erase(it);
            continue;
        }

        if(mGameObjectsToDelete.find(brick) != std::end(mGameObjectsToDelete))
        {
            it = mBricks.erase(it);
            continue;
        }

        for(const ComponentShared& component : brick->Components())
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
    return std::count_if(std::begin(mBricks), std::end(mBricks), [](const GameObjectShared& gameObjectShared) {
        return gameObjectShared && gameObjectShared->HasComponent<HealthComponent>();
    });
}

void Level::DebugDestroyAllBricks()
{
    mBricks.clear();
}

void Level::DebugDamageFirstBrick()
{
    if(mBricks.empty())
    {
        return;
    }

    for(GameObjectShared& brick : mBricks)
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

// CollisionListener
void Level::OnCollision(int /*l*/, int /*r*/)
{
    // auto brickIt1 = mBricks.find(l);
    // auto brickIt2 = mBricks.find(r);
    //// Bricks can't hit bricks
    // if(brickIt1 != mBricks.end() && brickIt2 != mBricks.end())
    //{
    //     return;
    // }
    //// At least one needs to be a brick
    // if(brickIt1 == mBricks.end() && brickIt2 == mBricks.end())
    //{
    //     return;
    // }

    // auto brickIt   = brickIt1 != mBricks.end() ? brickIt1 : brickIt2;
    // auto idOfBrick = brickIt == brickIt1 ? l : r;

    // brickIt->second.OnHit();
    // if(brickIt->second.Health() <= 0)
    //{
    //     mScoreReportingFunction(brickIt->second.ScoreValue());
    //     mEngine.GetCollisionWorld().RemoveBoxCollider(idOfBrick);
    //     mBricks.erase(idOfBrick);
    //     mNumBricksLeft--;
    // }
    // else
    //{
    //     mScoreReportingFunction(10); // 10 points for hitting a brick no matter the type
    // }
}