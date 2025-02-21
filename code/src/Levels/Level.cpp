// (C) king.com Ltd 2021
#include "Level.h"

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
    mNumBricksLeft = 0;
}

void Level::AddBrick(const glm::vec2& position, Brick::BrickType type, int hitPoints)
{
    Brick brick(mEngine, position, hitPoints, type);
    int brickId = mEngine.GetCollisionWorld().AddBoxCollider(position, glm::vec2(kBrickWidth, kBrickHeight), 1 << 1, 0);

    mBricks.emplace(std::pair(brickId, std::move(brick)));

    if(type != Brick::BRICK_SOLID)
    {
        mNumBricksLeft++;
    }
}

void Level::Render()
{
    for(auto brick : mBricks)
    {
        brick.second.Render();
    }
}

void Level::DestroyAllBricks()
{
    for(auto brickPair : mBricks)
    {
        mEngine.GetCollisionWorld().RemoveBoxCollider(brickPair.first);
    }
    mBricks.clear();
    mNumBricksLeft = 0;
}

// CollisionListener
void Level::OnCollision(int l, int r)
{
    auto brickIt1 = mBricks.find(l);
    auto brickIt2 = mBricks.find(r);
    // Bricks can't hit bricks
    if(brickIt1 != mBricks.end() && brickIt2 != mBricks.end())
    {
        return;
    }
    // At least one needs to be a brick
    if(brickIt1 == mBricks.end() && brickIt2 == mBricks.end())
    {
        return;
    }

    auto brickIt   = brickIt1 != mBricks.end() ? brickIt1 : brickIt2;
    auto idOfBrick = brickIt == brickIt1 ? l : r;

    brickIt->second.OnHit();
    if(brickIt->second.Health() <= 0)
    {
        mScoreReportingFunction(brickIt->second.ScoreValue());
        mEngine.GetCollisionWorld().RemoveBoxCollider(idOfBrick);
        mBricks.erase(idOfBrick);
        mNumBricksLeft--;
    }
    else
    {
        mScoreReportingFunction(10); // 10 points for hitting a brick no matter the type
    }
}

void Level::AddPaddle(GameObjectShared gameObject)
{
    mNewPaddles.emplace_back(gameObject);
}
