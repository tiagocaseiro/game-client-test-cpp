// (C) king.com Ltd 2021
#pragma once

#include "king/CollisionWorld.h"
#include "king/Engine.h"

const int kBrickWidth = 64;
const int kBrickHeight = 32;

class Brick
{
public:
    enum BrickType
    {
        BRICK_RED,
        BRICK_GREEN,
        BRICK_BLUE,
        BRICK_ORANGE,
        BRICK_GLASS,
        BRICK_GREY,
        BRICK_COUNTER,
        BRICK_SOLID
    };

    Brick(King::Engine& engine, const glm::vec2& position, int healthPoints, BrickType type);

    void OnHit();
    void Render();

    int Health() const
    {
        return mHealthPoints;
    }
    int ScoreValue() const
    {
        return mScoreValue;
    }

private:
    void UpdateBrickCounterTexture();

private:
    King::Engine& mEngine;

    BrickType mType;
    glm::vec2 mPosition;
    int mHealthPoints = 1;
    int mTexture;
    int mScoreValue;
};
