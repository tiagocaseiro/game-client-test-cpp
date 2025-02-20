// (C) king.com Ltd 2021
#pragma once

#include <king/CollisionWorld.h>
#include <king/Engine.h>

class Paddle
{
public:
    Paddle(King::Engine& engine);
    void ResetPosition();
    void Update();
    void Render() const;

    int GetColliderId() const
    {
        return mCollisionBoxId;
    }
    glm::vec2 GetCenter() const;

private:
    King::Engine& mEngine;

    glm::vec2 mPosition;
    King::BoxCollider* mCollisionBox;
    int mCollisionBoxId, mPaddleTxId;
};
