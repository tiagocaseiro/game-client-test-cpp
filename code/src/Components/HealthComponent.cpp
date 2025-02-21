#include "HealthComponent.h"

HealthComponent::HealthComponent(const u32 maxHealth) : mHealth(maxHealth)
{
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
}

bool HealthComponent::IsAlive() const
{
    return mHealth != 0;
}
