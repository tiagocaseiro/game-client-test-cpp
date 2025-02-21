#include "HealthComponent.h"
#include "SpriteComponent.h"

const std::string& HealthComponent::ID()
{
    static const std::string id = "HealthComponent";
    return id;
}

HealthComponent::HealthComponent(GameObjectRef owner, King::Engine& engine, const u32 maxHealth)
    : Component(owner, engine), mHealth(maxHealth), mMaxHealth(maxHealth)
{
}

ComponentShared HealthComponent::MakeComponent(GameObjectRef owner, King::Engine& engine,
                                               const std::vector<std::string>& parameters)
{
    if(parameters.empty())
    {
        return nullptr;
    }

    const u32 maxHealth = std::stoi(parameters.front());

    return std::shared_ptr<Component>(new HealthComponent(owner, engine, maxHealth));
}

void HealthComponent::DecrementHealth(const u32 dec)
{
    if(dec >= mHealth)
    {
        mHealth = 0;
        // mark for destruction
        return;
    }
    mHealth -= dec;

    GameObjectShared owner = mOwnerRef.lock();

    if(owner == nullptr)
    {
        return;
    }

    std::weak_ptr<SpriteComponent> spriteComponentRef = owner->FindComponent<SpriteComponent>();

    if(std::shared_ptr<SpriteComponent> spriteComponent = spriteComponentRef.lock())
    {
        spriteComponent->SetActiveTextureHandle(mMaxHealth - mHealth);
    }
}

bool HealthComponent::IsAlive() const
{
    return mHealth != 0;
}
