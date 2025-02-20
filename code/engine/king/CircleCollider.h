// (C) king.com Ltd 2021
#pragma once

#include "glm/vec2.hpp"
#include <vector>

namespace King {

struct CircleCollider {
	CircleCollider(const glm::vec2& position, float radius)
		: mPosition(position)
		, mRadius(radius)
	{}

	glm::vec2 mPosition;
	float mRadius;
};
}
