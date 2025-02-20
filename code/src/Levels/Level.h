// (C) king.com Ltd 2021
#pragma once

#include "Game/Brick.h"


#include "glm/vec2.hpp"
#include <king/Engine.h>
#include <king/CollisionWorld.h>

#include <map>
#include <functional>

class Level : King::CollisionWorld::CollisionListener {
public:
	using ScoreReportingFunction = std::function<void(int)>;
	Level(King::Engine& engine, ScoreReportingFunction scoreReportingFunction);
	~Level();

	void SetName(std::string name) { mName = name; }
	void SetBackground(std::string levelBackground) { mLevelBackground = levelBackground; }
	void SetNextLevelFilename(std::string nextLevelFilename) { mNextLevelFilename = nextLevelFilename; }


	std::string Name() const { return mName; }
	std::string Background() const { return mLevelBackground; }
	std::string NextLevelFilename() const { return mNextLevelFilename; }

	void Reset();
	void AddBrick(const glm::vec2& position, Brick::BrickType type, int hitPoints = 1);
	void Render();
	int NumBricksLeft() const { return mNumBricksLeft; }

	void DestroyAllBricks();

	// CollisionListener
	void OnCollision(int l, int r) override;
private:
	King::Engine& mEngine;
	ScoreReportingFunction mScoreReportingFunction;
	std::map<int, Brick> mBricks;
	int mNumBricksLeft;

	std::string mName;
	std::string mLevelBackground;
	std::string mNextLevelFilename;
};
