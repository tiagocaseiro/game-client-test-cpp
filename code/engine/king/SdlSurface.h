// (C) king.com Ltd 2021
#pragma once

#include <memory>

struct SDL_Surface;
struct SDL_Window;
struct SDL_Texture;
struct SDL_Renderer;

namespace King
{
struct Rect
{
    int x;
    int y;
    int w;
    int h;
};

class SdlWindow;

class SdlSurface
{
public:
    SdlSurface(const char* filename, SDL_Renderer& renderer);
    operator SDL_Surface*();

    int Width() const;
    int Height() const;

    void Blit(SdlSurface& target, int x, int y);
    void Blit(SdlWindow& window, float x, float y, float rotation, float scale);
    void BlitSub(SdlWindow& window, const Rect& source, const Rect& target);

private:
    std::unique_ptr<SDL_Surface, void (*)(SDL_Surface*)> mSurface;
    std::unique_ptr<SDL_Texture, void (*)(SDL_Texture*)> mTexture;
};
} // namespace King
