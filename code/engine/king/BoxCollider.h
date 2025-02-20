// (C) king.com Ltd 2021
#pragma once

#include "glm/vec2.hpp"
#include <vector>

namespace King
{
struct BoxCollider
{
    BoxCollider() = default;

    BoxCollider(const glm::vec2& topLeft, const glm::vec2& size)
        : mTopLeft(topLeft), mSize(size), mBottomRight(topLeft + size), mCenter(topLeft + (size * 0.5f))
    {
    }

    void Set(const glm::vec2& topLeft, const glm::vec2& size)
    {
        mTopLeft = topLeft;
        mSize = size;
        mBottomRight = topLeft + mSize;
        mCenter = mTopLeft + mSize * 0.5f;
    }

    glm::vec2 mTopLeft;
    glm::vec2 mSize;
    glm::vec2 mBottomRight;
    glm::vec2 mCenter;
};
} // namespace King