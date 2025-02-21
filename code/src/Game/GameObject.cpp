#include "GameObject.h"

#include <iostream>

#include <king/Engine.h>

#include "Levels/Level.h"

GameObject::GameObject(Level& level) : mLevel(level)
{
}

// GameObject::~GameObject()
//{
//     std::cout << __FUNCTION__ << std::endl;
// }

void GameObject::AddComponent(std::shared_ptr<Component> component)
{
    // Make it unique (?)
    mComponents.push_back(component);
}

const std::vector<std::shared_ptr<Component>> GameObject::Components() const
{
    return mComponents;
}

void GameObject::MarkForDeath()
{
    mLevel.MarkForDeath(shared_from_this());
}

Level& GameObject::GameLevel()
{
    return mLevel;
}
