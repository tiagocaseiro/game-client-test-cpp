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
                                               const std::unordered_map<std::string, std::string>& parameters)
{
    u32 maxHealth = 1;

    if(auto it = parameters.find("maxHealth"); it != std::end(parameters))
    {
        maxHealth = std::stoi(it->second);
    }

    return std::shared_ptr<Component>(new HealthComponent(owner, engine, maxHealth));
}

void HealthComponent::Decrement(const u32 dec)
{
    mHealth -= dec;

    mHealth = std::max(mHealth, 0);

    if(IsAlive() == false)
    {
        if(std::shared_ptr<GameObject> mOwner = mOwnerRef.lock())
        {
            mOwner->MarkForDeath();
        }
    }

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
