#pragma once

#include <optional>

#include "Component.h"
#include "TransformComponent.h"

class GameObject;

namespace King
{
class Engine;
}

class SpawnGameObjectOnDestructionComponent final : public Component
{

public:
    static const std::string& ID();
    static ComponentShared MakeComponent(GameObjectRef owner, King::Engine& engine,
                                         const std::unordered_map<std::string, std::string>& parameters);

private:
    void OnDestroyed() override;

    SpawnGameObjectOnDestructionComponent(GameObjectRef owner, King::Engine& engine);

private:
};