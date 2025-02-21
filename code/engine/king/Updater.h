// (C) king.com Ltd 2021

#pragma once

namespace King
{
struct Input;

class Updater
{
public:
    virtual void Update() = 0;

protected:
    ~Updater()
    {
    }
};
} // namespace King
