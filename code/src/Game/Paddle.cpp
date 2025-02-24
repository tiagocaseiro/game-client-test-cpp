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
    // Reset active power-up
    mPowerUp.reset();
}

void Paddle::Update()
{
    mPosition.x = mEngine.GetMouseX() - GetWidth() / 2;

    mPosition.x = glm::clamp(mPosition.x, 0.0f, static_cast<float>(kGameAreaWidth - GetWidth()));

    mCollisionBox->Set(mPosition, {GetWidth(), GetHeight()});

    if(mPowerUp && mPowerUp->timer)
    {
        *mPowerUp->timer -= mEngine.GetLastFrameSeconds();
        if(mPowerUp->timer <= 0)
        {
            mPowerUp.reset();
        }
    }
}

void Paddle::Render() const
{
    mEngine.Render(GetTextureHandle(), mPosition.x, mPosition.y, 0, 2);
}

glm::vec2 Paddle::GetCenter() const
{
    return mPosition + glm::vec2(GetWidth() / 2, GetHeight() / 2);
}

int Paddle::GetTextureHandle() const
{
    if(mPowerUp && mPowerUp->newTextureHandle)
    {
        return *mPowerUp->newTextureHandle;
    }
    return mPaddleTxId;
}

void Paddle::SetTextureHandle(const int textureHandle)
{
    mPaddleTxId = textureHandle;
}

void Paddle::SetPowerUp(Paddle::PowerUp powerUp)
{
    if(mPowerUp)
    {
        // Increase new timer with the already existing one's
        powerUp.timer = mPowerUp->timer.value_or(0) + powerUp.timer.value_or(0);
    }
    mPowerUp = powerUp;
}

int Paddle::GetWidth() const
{
    static constexpr int kPaddleWidth = 128;
    if(mPowerUp && mPowerUp->width)
    {
        return *mPowerUp->width;
    }

    return kPaddleWidth;
}

int Paddle::GetHeight() const
{
    static constexpr int kPaddleHeight = 32;
    if(mPowerUp && mPowerUp->height)
    {
        return *mPowerUp->height;
    }

    return kPaddleHeight;
}

std::optional<Paddle::PowerUp> Paddle::GetPowerUp() const
{
    return mPowerUp;
}