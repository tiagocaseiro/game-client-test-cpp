#pragma once

#include "Component.h"

class HealthComponent final : public Component
{
    using u32 = size_t;

public:
    static const std::string& ID();
    static ComponentShared MakeComponent(GameObjectRef owner, King::Engine& engine,
                                         const std::unordered_map<std::string, std::string>& parameters);

    void Decrement(const u32 dec = 1);

    bool IsAlive() const;

private:
    HealthComponent(GameObjectRef owner, King::Engine& engine, const u32 mHealth);

    int mHealth;
    const u32 mMaxHealth;
};