// (C) king.com Ltd 2021
#pragma once

#include "Game/Brick.h"

#include "glm/vec2.hpp"
#include <king/CollisionWorld.h>
#include <king/Engine.h>

#include <functional>
#include <map>

#include "Game/GameObject.h"

class Level : King::CollisionWorld::CollisionListener
{
public:
    using ScoreReportingFunction = std::function<void(int)>;
    Level(King::Engine& engine, ScoreReportingFunction scoreReportingFunction);
    ~Level();

    void SetName(std::string name)
    {
        mName = name;
    }
    void SetBackground(std::string levelBackground)
    {
        mLevelBackground = levelBackground;
    }
    void SetNextLevelFilename(std::string nextLevelFilename)
    {
        mNextLevelFilename = nextLevelFilename;
    }

    std::string Name() const
    {
        return mName;
    }
    std::string Background() const
    {
        return mLevelBackground;
    }
    std::string NextLevelFilename() const
    {
        return mNextLevelFilename;
    }

    void Reset();
    void AddBrick(const glm::vec2& position, Brick::BrickType type, int hitPoints = 1);
    void AddBrick(const GameObjectShared& brick);
    void Render();
    void Update();

    int NumBricksLeft() const;

    void DestroyAllBricks();

    // CollisionListener
    void OnCollision(int l, int r) override;

private:
    King::Engine& mEngine;
    ScoreReportingFunction mScoreReportingFunction;
    // std::map<int, Brick> mBricks;
    // int mNumBricksLeft;

    std::vector<GameObjectShared> mBricks;

    std::string mName;
    std::string mLevelBackground;
    std::string mNextLevelFilename;
};
