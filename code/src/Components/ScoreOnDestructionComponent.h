#pragma once

#include <optional>

#include "Component.h"
#include "TransformComponent.h"

class GameObject;

namespace King
{
    class Engine;
}

// Updates score when object is destroyed
class ScoreOnDestructionComponent final : public Component
{

public:
    static const std::string& ID();
    static ComponentShared MakeComponent(GameObjectRef owner, King::Engine& engine,
                                         const std::unordered_map<std::string, std::string>& parameters);

private:
    ScoreOnDestructionComponent(GameObjectRef owner, King::Engine& engine, const int score);
    void OnDestroyed() override;

private:
    const int mScore;
};