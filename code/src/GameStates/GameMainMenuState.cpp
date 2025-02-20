// (C) king.com Ltd 2021
#include "GameMainMenuState.h"
#include "Levels/LevelLoader.h"

namespace {
	float kBlinkTime = 0.8f;
}

GameMainMenuState::GameMainMenuState(King::Engine& engine, OnEndFunction onEndFunction)
	: mEngine(engine)
	, mBGTx(mEngine.LoadTexture("Start-screen.png"))
	, mShouldRenderText(true)
	, mOnEnd(onEndFunction)
{
}

void GameMainMenuState::Start() {
}

void GameMainMenuState::Update() {
	if (mEngine.GetMouseButtonPressed()) {
		mOnEnd();
	}
}

void GameMainMenuState::Render() {
	mEngine.Render(mBGTx, 0, 0, 0, 4.0f);

	mBlinkingTextTimer += mEngine.GetLastFrameSeconds();
	if (mBlinkingTextTimer > kBlinkTime) {
		mBlinkingTextTimer -= kBlinkTime;
		mShouldRenderText = !mShouldRenderText;
	}

	if (mShouldRenderText) {
		mEngine.WriteCentered("Click to start playing", 1280 / 2, 450, 4);
	}
}

void GameMainMenuState::End() {
}
