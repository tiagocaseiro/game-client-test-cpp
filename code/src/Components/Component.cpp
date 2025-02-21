#include "SpriteComponent.h"

#include "Component.h"

Component::Component(GameObjectRef owner, King::Engine& engine) : mOwnerRef(owner), mEngine(engine)
{
}

void Component::Render()
{
}

void Component::Update()
{
}
