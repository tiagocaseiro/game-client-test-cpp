#pragma once

#include <optional>

#include "Component.h"
#include "TransformComponent.h"

class GameObject;

namespace King
{
    class Engine;
}

// Spawn a new GameObject after destruction
class SpawnGameObjectOnDestructionComponent final : public Component
{

public:
    static const std::string& ID();
    static ComponentShared MakeComponent(GameObjectRef owner, King::Engine& engine,
                                         const std::unordered_map<std::string, std::string>& parameters);

private:
    void OnCreate() override;
    void OnDestroyed() override;

    SpawnGameObjectOnDestructionComponent(GameObjectRef owner, King::Engine& engine,
                                          const std::string& gameObjectTemplateId, const glm::vec2& offset);

private:
    std::weak_ptr<TransformComponent> mTransformComponent;

    const std::string mGameObjectTemplateId;
    const glm::vec2 mOffset;
};