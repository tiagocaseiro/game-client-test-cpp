#include "GameObject.h"

void GameObject::AddComponent(std::shared_ptr<Component> component)
{
    // Make it unique (?)
    mComponents.push_back(component);
}
