// (C) king.com Ltd 2021
#include "KingSdl.h"

#include <SDL.h>
#include <stdexcept>

namespace King
{
Sdl::Sdl(int flags)
{
    if(SDL_Init(flags) != 0)
    {
        throw std::runtime_error("Failed to init SDL");
    }
}
Sdl::~Sdl()
{
    SDL_Quit();
}
} // namespace King
