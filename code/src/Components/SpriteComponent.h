#pragma once

#include "Component.h"

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
    SpriteComponent(GameObjectRef owner, King::Engine& engine, std::vector<int> textureHandles);

public:
    const std::vector<int> mTextureHandles;
};