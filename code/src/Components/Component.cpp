#include "SpriteComponent.h"

#include "Component.h"

Component::Component(GameObjectRef owner, King::Engine& engine) : mOwnerRef(owner), mEngine(engine)
{
}

void Component::OnCreate()
{
}

void Component::Render()
{
}

void Component::Update()
{
}

void Component::OnComponentAdded(std::shared_ptr<Component>)
{
}

void Component::OnDestroyed()
{
}