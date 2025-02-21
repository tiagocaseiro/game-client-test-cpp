// (C) king.com Ltd 2021
#pragma once

class GameState
{
public:
    virtual ~GameState() {};
    virtual void Start() = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;
    virtual void End() = 0;
};
