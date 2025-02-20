// (C) king.com Ltd 2021
#pragma once

#include "Paddle.h"

#include <glm/vec2.hpp>
#include <king/CollisionWorld.h>

class Ball : public King::CollisionWorld::CollisionListener
{
public:
    Ball(King::Engine& engine);
    ~Ball();

    void SetStatic(bool isStatic);
    bool IsStatic() const;
    void SetPos(float posX, float posY);
    void SetVelocity(const glm::vec2& velocity);
    int ColliderId() const
    {
        return mBallColliderId;
    }

    void SetPaddle(Paddle* paddle)
    {
        mPaddle = paddle;
    }

    void Update();
    void Render() const;

    void SetLevelClear(bool levelClear)
    {
        mIsLevelClear = levelClear;
    }

private:
    void OnCollision(int l, int r) override;

    King::Engine& mEngine;
    int mBallTxId;
    int mBallColliderId;
    King::RigidBody* mRigidBody;
    Paddle* mPaddle;

    bool mIsLevelClear = false;
};
