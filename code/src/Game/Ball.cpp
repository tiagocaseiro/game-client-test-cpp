// (C) king.com Ltd 2021
#include "Ball.h"
#include <glm/glm.hpp>

#include <king/Engine.h>

namespace
{
    const float kSpeed       = 10.0f;
    const float kRadius      = 12.0f;
    const float kBounceLimit = 0.3f;
} // namespace

Ball::Ball(King::Engine& engine) : mEngine(engine), mBallTxId(mEngine.LoadTexture("Ball-yellow.png"))
{
    mEngine.GetCollisionWorld().AddCollisionListener(*this);
    mBallColliderId = mEngine.GetCollisionWorld().AddCircleCollider({}, kRadius, 1 << 2, 1 << 1);
    mRigidBody = mEngine.GetCollisionWorld().AddRigidBody(glm::normalize(glm::vec2(1, -1)) * kSpeed, {mBallColliderId});
}

Ball::~Ball()
{
    mEngine.GetCollisionWorld().RemoveRigidBody(mRigidBody);
    mEngine.GetCollisionWorld().RemoveCircleCollider(mBallColliderId);
    mEngine.GetCollisionWorld().RemoveCollisionListener(*this);
}

void Ball::SetStatic(bool isStatic)
{
    mRigidBody->mIsStatic = isStatic;
}

bool Ball::IsStatic() const
{
    return mRigidBody->mIsStatic;
}

void Ball::SetPos(float posX, float posY)
{
    auto collider         = mEngine.GetCollisionWorld().GetCircleCollider(mBallColliderId);
    collider->mPosition.x = posX;
    collider->mPosition.y = posY;
}

void Ball::SetVelocity(const glm::vec2& velocity)
{
    mRigidBody->mMovement = velocity;
}

void Ball::Update()
{
    if(mIsLevelClear)
    {
        float slowDown        = mEngine.GetLastFrameSeconds() * 500.0f;
        float currentSpeed    = glm::length(mRigidBody->mMovement);
        currentSpeed          = glm::max(0.0f, currentSpeed - slowDown);
        mRigidBody->mMovement = glm::normalize(mRigidBody->mMovement) * currentSpeed;
    }

    if(IsStatic())
    {
        glm::vec2 paddleCenter = mPaddle->GetCenter();
        SetPos(paddleCenter.x, paddleCenter.y - 32);
    }
}

void Ball::Render() const
{
    auto collider = mEngine.GetCollisionWorld().GetCircleCollider(mBallColliderId);
    mEngine.Render(mBallTxId, collider->mPosition.x - kRadius, collider->mPosition.y - kRadius, 0, 2);
}

void Ball::OnCollision(int l, int r)
{
    auto paddleColliderId = mPaddle->GetColliderId();
    bool hitPaddle        = paddleColliderId == l || paddleColliderId == r;
    bool hitBall          = mBallColliderId == l || mBallColliderId == r;

    if(hitPaddle && hitBall)
    {
        auto direction = glm::normalize(mRigidBody->mMovement);
        auto length    = glm::length(mRigidBody->mMovement);
        auto collider  = mEngine.GetCollisionWorld().GetCircleCollider(mBallColliderId);
        direction.x += (collider->mPosition.x - mPaddle->GetCenter().x) * 0.01f;
        mRigidBody->mMovement = glm::normalize(direction) * length;
    }

    if(hitBall && !mIsLevelClear)
    {
        auto direction = glm::normalize(mRigidBody->mMovement);
        auto length    = glm::length(mRigidBody->mMovement) + 1.0f;

        // Prevent very shallow bouncing angles
        float bounceMagnitude = glm::abs(glm::dot(glm::vec2(0.0f, 1.0f), direction));
        if(bounceMagnitude < kBounceLimit)
        {
            float x = sqrt(
                1 - std::pow(kBounceLimit,
                             2)); // Get x from a vector of lenght 1 and kBounceLimit as dot product with the up vector
            direction.x = x * (mRigidBody->mMovement.x > 0 ? 1.0f : -1.0f);
            direction.y = kBounceLimit * (mRigidBody->mMovement.y > 0 ? 1.0f : -1.0f);
        }

        mRigidBody->mMovement = direction * length;
    }
}
