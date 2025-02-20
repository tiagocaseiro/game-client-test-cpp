// (C) king.com Ltd 2021
#pragma once

#include "glm/vec2.hpp"
#include <vector>

namespace King {

struct RigidBody {
	RigidBody(const glm::vec2& movement, std::vector<int> colliders)
		: mMovement(movement)
		, mColliders(std::move(colliders))
		, mIsStatic(false)
	{}

	void AddCollider(int colliderId) { mColliders.push_back(colliderId); }

	glm::vec2 mMovement;
	std::vector<int> mColliders;
	bool mIsStatic;
};
}
