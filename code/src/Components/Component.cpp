#include "SpriteComponent.h"

#include "Component.h"

Component::Component(GameObjectRef owner, King::Engine& engine) : mOwner(owner), mEngine(engine)
{
}

void Component::Render()
{
}

void Component::Update()
{
}
