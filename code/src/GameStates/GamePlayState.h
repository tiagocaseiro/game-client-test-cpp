// (C) king.com Ltd 2021
#pragma once

#include "Game/Ball.h"
#include "Game/Paddle.h"
#include "GameState.h"
#include "Levels/Level.h"

#include <king/Engine.h>

class GamePlayState : public GameState, King::CollisionWorld::CollisionListener
{
public:
    using GameEndedFunction = std::function<void(bool, int)>;

    GamePlayState(King::Engine& engine, GameEndedFunction gameEndedFunction);
    void Start() override;
    void Update() override;
    void Render() override;
    void End() override;

    void ResetScore();
    void SetLevelFilename(std::string levelFilename)
    {
        mLevelFilename = levelFilename;
    }

    void MarkForDeath(const std::shared_ptr<GameObject>& gameObject);
    void AddGameObject(const std::shared_ptr<GameObject>& gameObject);
    void UpdateScore(int score);

    Paddle* GetPaddle();

private:
    void RenderUI();
    void RenderUIIndicator(int y, std::string title, std::string text);

private:
    //
    void OnCollision(int l, int r) override;
    void ResetPaddleAndBall();
    void UpdateGameObjects();
    void RenderGameObjects();

    King::Engine& mEngine;
    GameEndedFunction mGameEndedFunction;
    LevelShared mLevel;
    int mBGTx, mPanelTx, mTextFrameTx;

    int mScore;
    std::unique_ptr<Paddle> mPaddle;
    std::unique_ptr<Ball> mBall;

    std::vector<GameObjectShared> mGameObjects;
    std::set<GameObjectShared> mGameObjectsToDelete;

    int mIdOfBottomCollider;
    int mNumBallsLeft;
    std::vector<int> mColliders;

    std::string mLevelFilename;
    bool mLevelClear;
};
