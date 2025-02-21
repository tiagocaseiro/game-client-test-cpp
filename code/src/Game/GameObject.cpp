#include "GameObject.h"

void GameObject::AddComponent(std::shared_ptr<Component> component)
{
    // Make it unique (?)
    mComponents.push_back(component);
}

const std::vector<std::shared_ptr<Component>> GameObject::Components() const
{
    return mComponents;
}
