// (C) king.com Ltd 2021
#pragma once

#include "Game/Brick.h"

#include "glm/vec2.hpp"
#include <king/CollisionWorld.h>
#include <king/Engine.h>

#include <functional>
#include <map>
#include <set>

#include "Game/GameObject.h"

class Level
{
public:
    using ScoreReportingFunction = std::function<void(int)>;
    Level(King::Engine& engine);

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

private:
    King::Engine& mEngine;
    ScoreReportingFunction mScoreReportingFunction;

    std::string mName;
    std::string mLevelBackground;
    std::string mNextLevelFilename;
};

using LevelShared = std::shared_ptr<Level>;
using LevelRef    = std::weak_ptr<Level>;