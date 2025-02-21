#include "SpriteComponent.h"

#include <sstream>

#include <king/Engine.h>

#include "Component.h"

SpriteComponent::SpriteComponent(std::weak_ptr<GameObject> owner, King::Engine& engine, std::vector<int> textureHandles)
    : Component(owner, engine), mTextureHandles(textureHandles)
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
