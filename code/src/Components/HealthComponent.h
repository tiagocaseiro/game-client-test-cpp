#pragma once

#include "Component.h"

class HealthComponent : public Component
{
    using u32 = size_t;

public:
    static const std::string& ID();
    static ComponentShared MakeComponent(GameObjectRef owner, King::Engine& engine,
                                         const std::unordered_map<std::string, std::string>& parameters);

    void DecrementHealth(const u32 dec);
    bool IsAlive() const;

private:
    HealthComponent(GameObjectRef owner, King::Engine& engine, const u32 dec);
    u32 mHealth;
    const u32 mMaxHealth;
};