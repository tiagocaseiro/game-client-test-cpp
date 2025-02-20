// (C) king.com Ltd 2021
#include "GameEndState.h"

GameEndState::GameEndState(King::Engine& engine, OnEndFunction onEndFunction)
	: mEngine(engine)
	, mBGTx(mEngine.LoadTexture("Victory.png"))
	, mWon(false)
	, mScore(0)
	, mOnEnd(onEndFunction)

{
}

void GameEndState::SetGameResult(bool won, int score) {
    mScore = score;
    mWon = won;
    
    mBGTx = mEngine.LoadTexture(won? "Victory.png" : "Game-over.png");
}

void GameEndState::Start() {
}

void GameEndState::Update() {
	if (mEngine.GetMouseButtonPressed()) {
		mOnEnd();
	}
}

void GameEndState::Render() {
	mEngine.Render(mBGTx, 0, 0, 0, 4.0f);

	auto score = std::to_string(mScore);
	if(mWon) {
		mEngine.Write("CONGRATULATIONS!", 100, 160, 4.0f);
		mEngine.Write(("Your score: " + score).c_str(), 100, 260, 4.0f);
	}
	else {
		mEngine.Write("Better luck next time!", 380, 180, 4.0f);
		mEngine.Write(("Your score: " + score).c_str(), 280, 660, 4.0f);
	}
}

void GameEndState::End() {
}
