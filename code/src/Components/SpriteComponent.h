#pragma once

#include <optional>

#include "Component.h"
#include "TransformComponent.h"

class GameObject;

namespace King
{
class Engine;
}

class SpriteComponent : public Component
{

public:
    static const std::string& ID();
    static ComponentShared MakeComponent(GameObjectRef owner, King::Engine& engine,
                                         const std::vector<std::string>& parameters);

private:
    void Render() override;
    void Update() override;

    SpriteComponent(GameObjectRef owner, King::Engine& engine, std::vector<int> textureHandles);

public:
    std::weak_ptr<TransformComponent> mTransformComponentRef;
    const std::vector<int> mTextureHandles;
    std::optional<int> mActiveTextureHandle;
};