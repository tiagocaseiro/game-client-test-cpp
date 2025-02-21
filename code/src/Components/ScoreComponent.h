#pragma once

#include <king/CollisionWorld.h>

#include "glm/vec2.hpp"

#include "Component.h"

class CollisionBoxComponent;
class HealthComponent;

class ScoreComponent : public Component
{
public:
    static const std::string& ID();
    static ComponentShared MakeComponent(GameObjectRef owner, King::Engine& engine,
                                         const std::unordered_map<std::string, std::string>& parameters);
    static int DefaultScore();

    int ScoreValue() const;

private:
    ScoreComponent(GameObjectRef owner, King::Engine& engine, const int score);

    const int mScore;
};