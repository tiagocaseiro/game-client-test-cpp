// (C) king.com Ltd 2021
#pragma once

#include <memory>
#include <optional>

#include <king/CollisionWorld.h>
#include <king/Engine.h>

class SetTimedPaddleDataComponent;

class Paddle
{

public:
    struct PowerUp
    {
        std::optional<float> timer;
        std::optional<int> width;
        std::optional<int> height;
        std::optional<int> newTextureHandle;
    };
    Paddle(King::Engine& engine);
    void Reset();
    void Update();
    void Render() const;

    int GetColliderId() const
    {
        return mCollisionBoxId;
    }
    glm::vec2 GetCenter() const;

    int GetTextureHandle() const;

    void SetTextureHandle(const int textureHandle);
    void Paddle::SetPowerUp(PowerUp powerUp);

    std::optional<PowerUp> GetPowerUp() const;

private:
    int GetWidth() const;
    int GetHeight() const;

    King::Engine& mEngine;

    glm::vec2 mPosition;
    King::BoxCollider* mCollisionBox;
    int mCollisionBoxId, mPaddleTxId;
    std::optional<PowerUp> mPowerUp;
};
