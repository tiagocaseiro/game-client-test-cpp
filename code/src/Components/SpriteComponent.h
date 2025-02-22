#pragma once

#include <optional>

#include "Component.h"
#include "TransformComponent.h"

class GameObject;

namespace King
{
    class Engine;
}

class SpriteComponent final : public Component
{

public:
    static const std::string& ID();
    static ComponentShared MakeComponent(GameObjectRef owner, King::Engine& engine,
                                         const std::unordered_map<std::string, std::string>& parameters);

    void SetActiveTextureHandle(size_t index);

private:
    void OnCreate() override;
    void Render() override;

    SpriteComponent(GameObjectRef owner, King::Engine& engine, std::vector<int> textureHandles);

private:
    std::weak_ptr<TransformComponent> mTransformComponentRef;
    const std::vector<int> mTextureHandles;
    std::optional<int> mActiveTextureHandle;
};