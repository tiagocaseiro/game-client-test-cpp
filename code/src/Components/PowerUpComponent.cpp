#include "PowerUpComponent.h"

#include <king/Engine.h>

#include "CollisionComponent.h"
#include "Game/GameObject.h"
#include "Game/Paddle.h"
#include "GameStates/GamePlayState.h"
#include "HealthComponent.h"

const std::string& PaddleResizePowerUpComponent::ID()
{
    static const std::string id = "PaddleResizePowerUpComponent";
    return id;
}

ComponentShared PaddleResizePowerUpComponent::MakeComponent(
    GameObjectRef owner, King::Engine& engine, const std::unordered_map<std::string, std::string>& parameters)
{
    std::optional<float> timer;
    std::optional<int> width;
    std::optional<int> height;
    std::optional<int> newTextureHandle;

    if(auto it = parameters.find("timer"); it != std::end(parameters))
    {
        timer = std::stof(it->second);
    }

    if(auto it = parameters.find("width"); it != std::end(parameters))
    {
        width = std::stoi(it->second);
    }

    if(auto it = parameters.find("height"); it != std::end(parameters))
    {
        height = std::stoi(it->second);
    }

    if(auto it = parameters.find("sprite"); it != std::end(parameters))
    {

        newTextureHandle = engine.LoadTexture(it->second.c_str());
    }

    return std::shared_ptr<Component>(
        new PaddleResizePowerUpComponent(owner, engine, timer, width, height, newTextureHandle));
}

PaddleResizePowerUpComponent::PaddleResizePowerUpComponent(GameObjectRef owner, King::Engine& engine,
                                                           const std::optional<float> timer,
                                                           const std::optional<int> width,
                                                           const std::optional<int> height,
                                                           const std::optional<int> newTextureHandle)
    : PowerUpComponent(owner, engine),
      mTimer(timer),
      mWidth(width),
      mHeight(height),
      mNewTextureHandle(newTextureHandle)

{
    mEngine.GetCollisionWorld().AddCollisionListener(*this);
}

PaddleResizePowerUpComponent::~PaddleResizePowerUpComponent()
{
    mEngine.GetCollisionWorld().RemoveCollisionListener(*this);
}

void PaddleResizePowerUpComponent::OnCreate()
{

    mCollisionComponentRef = GetOwnerComponent<CollisionComponent>();
}

void PaddleResizePowerUpComponent::OnCollision(int l, int r)
{
    std::shared_ptr<CollisionComponent> collisionComponent = mCollisionComponentRef.lock();
    std::shared_ptr<GameObject> owner                      = mOwnerRef.lock();

    if(collisionComponent == nullptr || owner == nullptr)
    {
        return;
    }
    Paddle* paddle = owner->GameState().GetPaddle();

    if(paddle == nullptr)
    {
        return;
    }

    const int colliderId = collisionComponent->ColliderId();

    if(l != colliderId && r != colliderId)
    {
        return;
    }

    if(l != paddle->GetColliderId() && r != paddle->GetColliderId())
    {
        return;
    }

    paddle->SetPowerUp({mTimer, mWidth, mHeight, mNewTextureHandle});
}
