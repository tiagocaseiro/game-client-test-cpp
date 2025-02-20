// (C) king.com Ltd 2021
#pragma once

#include "GameState.h"
#include "Game/Paddle.h"
#include "Game/Ball.h"
#include "Levels/Level.h"

#include <king/Engine.h>

#include <functional>

class GameEndState : public GameState {
public:
	using OnEndFunction = std::function<void()>;

	GameEndState(King::Engine& engine, OnEndFunction onEndFunction);
	void SetGameResult(bool won, int score);
	
	void Start() override;
	void Update() override;
	void Render() override;
	void End() override;

	bool Won() { return mWon; }

private:
	King::Engine& mEngine;
	int mBGTx;

	bool mWon;
	int mScore;

	OnEndFunction mOnEnd;
};
