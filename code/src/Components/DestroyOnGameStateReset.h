#pragma once

#include <king/CollisionWorld.h>

#include "glm/vec2.hpp"

#include "Component.h"

class CollisionComponent;
class Paddle;

// Mark GameObject for destruction whenever GameState is reset (done by GameState)
class DestroyOnGameStateReset final : public Component

{
public:
    static const std::string& ID();
    static ComponentShared MakeComponent(GameObjectRef owner, King::Engine& engine,
                                         const std::unordered_map<std::string, std::string>& parameters);

private:
    using Component::Component;
};