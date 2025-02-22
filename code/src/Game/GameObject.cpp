#include "GameObject.h"

#include <iostream>

#include <king/Engine.h>

#include "Components/Component.h"
#include "Components/ComponentInit.h"
#include "Levels/Level.h"

GameObjectShared GameObject::MakeGameObject(Level& level, const GameObjectTemplate& gameObjectTemplate)
{
    GameObjectShared gameObject = std::shared_ptr<GameObject>(new GameObject(level));

    if(gameObject == nullptr)
    {
        return nullptr;
    }

    for(const auto& componentInitData : gameObjectTemplate)
    {
        const std::function<ComponentShared(GameObjectRef)>& componentInitFunc = componentInitData.second;
        gameObject->AddComponent(componentInitFunc(gameObject));
    }

    for(const ComponentShared& component : gameObject->mComponents)
    {
        component->OnCreate();
    }

    return gameObject;
}

GameObject::GameObject(Level& level) : mLevel(level)
{
}

// GameObject::~GameObject()
//{
//     std::cout << __FUNCTION__ << std::endl;
// }

void GameObject::AddComponent(std::shared_ptr<Component> newComponent)
{
    // Make it unique (?)
    mComponents.push_back(newComponent);

    for(const std::shared_ptr<Component>& component : mComponents)
    {
        component->OnComponentAdded(newComponent);
    }
}

const std::vector<std::shared_ptr<Component>> GameObject::Components() const
{
    return mComponents;
}

void GameObject::MarkForDeath()
{
    mLevel.MarkForDeath(shared_from_this());
}

const Level& GameObject::GameLevel()
{
    return mLevel;
}
