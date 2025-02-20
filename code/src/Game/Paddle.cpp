// (C) king.com Ltd 2021
#include "Paddle.h"
#include "glm/glm.hpp"
const int kPaddleWidth = 128;
const int kPaddleHeight = 32;
const int kGameAreaWidth = 1024;

Paddle::Paddle(King::Engine& engine)
	: mEngine(engine)
	, mPaddleTxId(engine.LoadTexture("Paddle.png"))
{
	mCollisionBoxId = mEngine.GetCollisionWorld().AddBoxCollider(glm::bvec2(0, 0),
															   glm::vec2(kPaddleWidth, kPaddleHeight),
															   1 << 1,
															   0);
	mCollisionBox = mEngine.GetCollisionWorld().GetBoxCollider(mCollisionBoxId);
	ResetPosition();
}

void Paddle::ResetPosition() {
	mPosition.x = kGameAreaWidth / 2 - kPaddleWidth / 2;
	mPosition.y = 900;
}

void Paddle::Update() {
	mPosition.x = mEngine.GetMouseX() - kPaddleWidth / 2;
	
	mPosition.x = glm::clamp(mPosition.x, 0.0f, static_cast<float>(kGameAreaWidth - kPaddleWidth));

	mCollisionBox->Set(mPosition, mCollisionBox->mSize);
}

void Paddle::Render() const {
	mEngine.Render(mPaddleTxId, mPosition.x, mPosition.y, 0, 2);
}

glm::vec2 Paddle::GetCenter() const {
	return mPosition + glm::vec2(kPaddleWidth / 2, kPaddleHeight / 2);
}
