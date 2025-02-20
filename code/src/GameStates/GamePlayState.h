// (C) king.com Ltd 2021
#pragma once

#include "GameState.h"
#include "Game/Paddle.h"
#include "Game/Ball.h"
#include "Levels/Level.h"

#include <king/Engine.h>

class GamePlayState : public GameState, King::CollisionWorld::CollisionListener {
public:
	using GameEndedFunction = std::function<void(bool, int)>;

	GamePlayState(King::Engine& engine, GameEndedFunction gameEndedFunction);
	void Start() override;
	void Update() override;
	void Render() override;
	void End() override;

	void ResetScore();
	void SetLevelFilename(std::string levelFilename) { mLevelFilename = levelFilename; }

private:
	void RenderUI();
	void RenderUIIndicator(int y, std::string title, std::string text);

private:
	//
	void OnCollision(int l, int r) override;
	void ResetPaddleAndBall();

	King::Engine& mEngine;
	GameEndedFunction mGameEndedFunction;
	std::unique_ptr<Level> mLevel;
	int mBGTx, mPanelTx, mTextFrameTx;

	int mScore;
	std::unique_ptr<Paddle> mPaddle;
	std::unique_ptr<Ball> mBall;

	int mIdOfBottomCollider;
	int mNumBallsLeft;
	std::vector<int> mColliders;

	std::string mLevelFilename;
	bool mLevelClear;
};
