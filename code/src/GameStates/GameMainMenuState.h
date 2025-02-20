// (C) king.com Ltd 2021
#pragma once

#include "Game/Ball.h"
#include "Game/Paddle.h"
#include "GameState.h"
#include "Levels/Level.h"

#include <king/CollisionWorld.h>
#include <king/Engine.h>

#include <functional>

class GameMainMenuState : public GameState
{
public:
    using OnEndFunction = std::function<void()>;

    GameMainMenuState(King::Engine& engine, OnEndFunction onEndFunction);
    void Start() override;
    void Update() override;
    void Render() override;
    void End() override;

private:
    King::Engine& mEngine;
    int mBGTx;

    bool mShouldRenderText;
    float mBlinkingTextTimer = 0;
    OnEndFunction mOnEnd;
};
