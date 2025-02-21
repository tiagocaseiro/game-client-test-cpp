#pragma once

class HealthComponent
{
    using u32 = size_t;

public:
    explicit HealthComponent(const u32 maxHealth);

    void DecrementHealth(const u32 dec);

    bool IsAlive() const;

private:
    u32 mHealth;
};