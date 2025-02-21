#include "SpriteComponent.h"

#include <sstream>

#include <king/Engine.h>

#include "Component.h"
#include "TransformComponent.h"

SpriteComponent::SpriteComponent(std::weak_ptr<GameObject> owner, King::Engine& engine, std::vector<int> textureHandles)
    : Component(owner, engine),
      mTextureHandles(textureHandles),
      mActiveTextureHandle(textureHandles.empty() ? std::nullopt : std::make_optional(textureHandles.front()))
{
}

const std::string& SpriteComponent::ID()
{
    static const std::string id = "SpriteComponent";
    return id;
}

ComponentShared SpriteComponent::MakeComponent(GameObjectRef owner, King::Engine& engine,
                                               const std::vector<std::string>& parameters)
{
    if(parameters.empty())
    {
        return nullptr;
    }

    std::vector<int> textureHandles;
    textureHandles.reserve(parameters.size());

    for(const std::string& texturePath : parameters)
    {
        textureHandles.push_back(engine.LoadTexture(texturePath.c_str()));
    }

    return std::shared_ptr<Component>(new SpriteComponent(owner, engine, textureHandles));
}

void SpriteComponent::Render()
{
    if(mActiveTextureHandle.has_value() == false)
    {
        return;
    }

    if(mTransformComponentRef.expired())
    {
        return;
    }

    std::shared_ptr<TransformComponent> transformComponent = mTransformComponentRef.lock();

    mEngine.Render(*mActiveTextureHandle, transformComponent->GetPosition().x, transformComponent->GetPosition().y,
                   transformComponent->GetRotation(), transformComponent->GetScale());
}

void SpriteComponent::Update()
{

    if(mTransformComponentRef.expired() == false)
    {
        return;
    }

    if(mOwner.expired())
    {
        return;
    }
    mTransformComponentRef = mOwner.lock()->FindComponent<TransformComponent>();
}