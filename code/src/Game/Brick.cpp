// (C) king.com Ltd 2021
#include "Brick.h"

Brick::Brick(King::Engine& engine, const glm::vec2& position, int healthPoints, BrickType type)
    : mEngine(engine), mType(type), mPosition(position), mHealthPoints(healthPoints), mScoreValue(mHealthPoints * 100)
{
    switch(type)
    {
        case BRICK_RED:
            mTexture = mEngine.LoadTexture("Brick-red.png");
            break;
        case BRICK_GREEN:
            mTexture = mEngine.LoadTexture("Brick-green.png");
            break;
        case BRICK_BLUE:
            mTexture = mEngine.LoadTexture("Brick-blue.png");
            break;
        case BRICK_ORANGE:
            mTexture = mEngine.LoadTexture("Brick-orange.png");
            break;
        case BRICK_GLASS:
            mTexture = mEngine.LoadTexture("Brick-glass.png");
            break;
        case BRICK_GREY:
            mTexture = mEngine.LoadTexture("Brick-grey.png");
            break;
        case BRICK_SOLID:
            mTexture = mEngine.LoadTexture("Brick-solid.png");
            break;
        case BRICK_COUNTER:
            UpdateBrickCounterTexture();
            break;
    }
}

void Brick::OnHit()
{
    if(mType == BRICK_SOLID)
    {
        return;
    }
    --mHealthPoints;
    if(mHealthPoints <= 0)
    {
        mHealthPoints = 0;
        return;
    }

    if(mType == BRICK_COUNTER)
    {
        UpdateBrickCounterTexture();
    }

    if(mType == BRICK_GREY && mHealthPoints == 1)
    {
        mTexture = mEngine.LoadTexture("Brick-grey-broken.png");
    }
}

void Brick::Render()
{
    mEngine.Render(mTexture, mPosition.x, mPosition.y, 0, 2);
}

void Brick::UpdateBrickCounterTexture()
{
    switch(mHealthPoints)
    {
        case 1:
            mTexture = mEngine.LoadTexture("Brick-counter-01.png");
            return;
        case 2:
            mTexture = mEngine.LoadTexture("Brick-counter-02.png");
            return;
        case 3:
            mTexture = mEngine.LoadTexture("Brick-counter-03.png");
            return;
        case 4:
            mTexture = mEngine.LoadTexture("Brick-counter-04.png");
            return;
        case 5:
            mTexture = mEngine.LoadTexture("Brick-counter-05.png");
            return;
        case 6:
            mTexture = mEngine.LoadTexture("Brick-counter-06.png");
            return;
        default:
            mTexture = mEngine.LoadTexture("Brick-counter-07.png");
            return;
    }
}
