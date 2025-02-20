// (C) king.com Ltd 2021
#pragma once

#include <memory>
#include <string>

namespace King
{

class SdlSurface;
class SdlWindow;

class TextureManager
{
public:
    TextureManager(SdlWindow& window);
    ~TextureManager();

    int LoadTexture(const std::string& path);
    SdlSurface* GetTexture(int id);

private:
    class TextureManagerImpl;
    std::unique_ptr<TextureManagerImpl> mImpl;
};

} // namespace King
