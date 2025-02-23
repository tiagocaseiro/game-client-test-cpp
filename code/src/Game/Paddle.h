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
    struct TimedData
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
    void Paddle::SetTimedData(TimedData timedData);

    std::optional<TimedData> GetTimedData();

private:
    void InitCollisionData();

    int GetWidth() const;
    int GetHeight() const;

    King::Engine& mEngine;

    glm::vec2 mPosition;
    King::BoxCollider* mCollisionBox;
    int mCollisionBoxId, mPaddleTxId;
    std::optional<TimedData> mTimedData;
};
