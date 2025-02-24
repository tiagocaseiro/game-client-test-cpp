// (C) king.com Ltd 2021
#include "Paddle.h"
#include "glm/glm.hpp"

const int kGameAreaWidth = 1024;

Paddle::Paddle(King::Engine& engine)
    : mEngine(engine), mPaddleTxId(engine.LoadTexture("Paddle.png")), mCollisionBoxId(-1), mCollisionBox(nullptr)
{
    mCollisionBoxId = mEngine.GetCollisionWorld().AddBoxCollider(glm::bvec2(0, 0), glm::vec2(GetWidth(), GetHeight()),
                                                                 1 << 1, 1 << 4);
    mCollisionBox   = mEngine.GetCollisionWorld().GetBoxCollider(mCollisionBoxId);
    Reset();
}

void Paddle::Reset()
{
    mPosition.x = kGameAreaWidth / 2 - GetWidth() / 2;
    mPosition.y = 900;
    mTimedData.reset();
}

void Paddle::Update()
{
    mPosition.x = mEngine.GetMouseX() - GetWidth() / 2;

    mPosition.x = glm::clamp(mPosition.x, 0.0f, static_cast<float>(kGameAreaWidth - GetWidth()));

    mCollisionBox->Set(mPosition, mCollisionBox->mSize);

    if(mTimedData == std::nullopt)
    {
        return;
    }
    if(mTimedData->timer)
    {
        *mTimedData->timer -= mEngine.GetLastFrameSeconds();
        if(mTimedData->timer <= 0)
        {
            mTimedData.reset();
        }
    }
}

void Paddle::Render() const
{
    int textureHandle = mPaddleTxId;

    if(mTimedData && mTimedData->newTextureHandle)
    {
        textureHandle = *mTimedData->newTextureHandle;
    }

    mEngine.Render(textureHandle, mPosition.x, mPosition.y, 0, 2);
}

glm::vec2 Paddle::GetCenter() const
{
    return mPosition + glm::vec2(GetWidth() / 2, GetHeight() / 2);
}

int Paddle::GetTextureHandle() const
{
    return mPaddleTxId;
}

void Paddle::SetTextureHandle(const int textureHandle)
{
    mPaddleTxId = textureHandle;
}

void Paddle::SetTimedData(Paddle::TimedData timedData)
{
    if(mTimedData)
    {
        timedData.timer = mTimedData->timer.value_or(0) + timedData.timer.value_or(0);
    }
    mTimedData = timedData;
    // assert(!"Update COLLISION BOX!");
    if(mCollisionBox)
    {
    }
}

int Paddle::GetWidth() const
{
    static constexpr int kPaddleWidth = 128;
    if(mTimedData && mTimedData->width)
    {
        return *mTimedData->width;
    }

    return kPaddleWidth;
}

int Paddle::GetHeight() const
{
    static constexpr int kPaddleHeight = 32;
    if(mTimedData && mTimedData->height)
    {
        return *mTimedData->height;
    }

    return kPaddleHeight;
}

std::optional<Paddle::TimedData> Paddle::GetTimedData()
{
    return mTimedData;
}