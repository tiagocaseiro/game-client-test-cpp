// (C) king.com Ltd 2021
#include "Engine.h"

#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "CollisionWorld.h"
#include "Font.h"
#include "KingSdl.h"
#include "SdlSurface.h"
#include "SdlWindow.h"
#include "TextureManager.h"
#include "Updater.h"

namespace King
{
static const int WindowWidth = 1280;
static const int WindowHeight = 960;
static const float MaxFrameTicks = 300.0f;
static const float TextScale = 0.5f;

struct Engine::EngineImplementation
{
    Sdl mSdl;
    SdlWindow mSdlWindow;
    TextureManager mTextureManager;
    CollisionWorld mCollisionWorld;
    int mFontTextureId;
    float mElapsedTicks;
    float mLastFrameSeconds;
    Updater* mUpdater;

    float mMouseX;
    float mMouseY;
    bool mKeyStates[256];
    bool mMouseButtonDown;
    bool mMouseButtonPressed;
    bool mQuit;

    EngineImplementation()
        : mSdl(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_NOPARACHUTE), mSdlWindow(WindowWidth, WindowHeight),
          mTextureManager(mSdlWindow), mElapsedTicks(static_cast<float>(SDL_GetTicks())),
          mLastFrameSeconds(1.0f / 60.0f), mUpdater(nullptr), mMouseX(WindowWidth * 0.5f), mMouseY(WindowHeight * 0.5f),
          mMouseButtonDown(false), mQuit(false)
    {
        for(int i = 0; i < 256; ++i)
        {
            mKeyStates[i] = false;
        }
    }

    void Start();
    void ParseEvents();
};

Engine::Engine(const char* assetsDirectory) : mAssetsDirectory(assetsDirectory), mPimpl(new EngineImplementation)
{
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetSwapInterval(1);
    mPimpl->mFontTextureId = LoadTexture("/Martorelius.png");
}

Engine::~Engine()
{
}

CollisionWorld& Engine::GetCollisionWorld()
{
    return mPimpl->mCollisionWorld;
}

float Engine::GetLastFrameSeconds() const
{
    return mPimpl->mLastFrameSeconds;
}

float Engine::GetMouseX() const
{
    return mPimpl->mMouseX;
}

float Engine::GetMouseY() const
{
    return mPimpl->mMouseY;
}

bool Engine::GetMouseButtonDown() const
{
    return mPimpl->mMouseButtonDown;
}

bool Engine::GetMouseButtonPressed() const
{
    return mPimpl->mMouseButtonPressed;
}

bool Engine::GetKeyDown(int keyCode) const
{
    if(keyCode < 0 || keyCode > 255)
    {
        return false;
    }
    return mPimpl->mKeyStates[keyCode];
}

void Engine::SetCursorVisible(bool visible)
{
    SDL_ShowCursor(visible);
}

void Engine::Quit()
{
    mPimpl->mQuit = true;
}

void Engine::Start(Updater& updater)
{
    mPimpl->mUpdater = &updater;
    mPimpl->mSdlWindow.Show();
    mPimpl->Start();
}

int Engine::LoadTexture(const char* name)
{
    const std::string path = mAssetsDirectory + "/" + name;
    return mPimpl->mTextureManager.LoadTexture(path);
}

int Engine::GetTextureHeight(int id) const
{
    auto tx = mPimpl->mTextureManager.GetTexture(id);
    return tx ? tx->Height() : 0;
}

int Engine::GetTextureWidth(int id) const
{
    auto tx = mPimpl->mTextureManager.GetTexture(id);
    return tx ? tx->Width() : 0;
}

void Engine::Render(int id, float x, float y, float rotation, float scale)
{
    if(auto tx = mPimpl->mTextureManager.GetTexture(id))
    {
        tx->Blit(mPimpl->mSdlWindow, x, y, rotation, scale);
    }
}

Glyph& FindGlyph(char c)
{
    auto found = std::lower_bound(std::begin(Font), std::end(Font), c);
    if(found == std::end(Font) || c < *found)
    {
        found = std::lower_bound(std::begin(Font), std::end(Font), static_cast<int>('_'));
    }
    return *found;
}

float Engine::CalculateStringWidth(const char* text) const
{
    int advance = 0;
    for(; *text; ++text)
    {
        Glyph& g = FindGlyph(*text);
        advance += g.advance;
    }
    return advance * TextScale;
}

void Engine::Write(const char* text, float x, float y, float scale)
{
    if(auto tx = mPimpl->mTextureManager.GetTexture(mPimpl->mFontTextureId))
    {
        SDL_SetRenderDrawBlendMode(&mPimpl->mSdlWindow.Renderer(), SDL_BlendMode::SDL_BLENDMODE_BLEND);
        int advance = 0;
        for(; *text; ++text)
        {
            Glyph& g = FindGlyph(*text);

            Rect source;
            source.x = g.x;
            source.y = g.y;
            source.w = g.width;
            source.h = g.height;

            Rect target;
            target.x = int(x + advance);
            target.y = int(y + g.yoffset * scale);
            target.w = g.width * scale;
            target.h = g.height * scale;

            tx->BlitSub(mPimpl->mSdlWindow, source, target);
            advance += g.advance * scale;
        }
        SDL_SetRenderDrawBlendMode(&mPimpl->mSdlWindow.Renderer(), SDL_BlendMode::SDL_BLENDMODE_NONE);
    }
}

void Engine::WriteCentered(const char* text, float x, float y, float scale)
{
    int width = 0;
    for(int i = 0; text[i] != '\0'; ++i)
    {
        Glyph& g = FindGlyph(text[i]);
        width += g.advance * scale;
    }

    Write(text, x - width / 2, y, scale);
}

int Engine::GetWidth() const
{
    return WindowWidth;
}

int Engine::GetHeight() const
{
    return WindowHeight;
}

// =====================================================================================

void Engine::EngineImplementation::Start()
{
    while(!mQuit)
    {
        SDL_RenderClear(&mSdlWindow.Renderer());
        SDL_GL_SwapWindow(mSdlWindow);
        ParseEvents();

        float currentTicks = static_cast<float>(SDL_GetTicks());
        float lastFrameTicks = currentTicks - mElapsedTicks;
        mElapsedTicks = currentTicks;

        lastFrameTicks = std::min(lastFrameTicks, MaxFrameTicks);
        mLastFrameSeconds = lastFrameTicks * 0.001f;

        mCollisionWorld.ProcessCollisions(mLastFrameSeconds);

        if(mUpdater)
        {
            mUpdater->Update();
        }
        SDL_RenderPresent(&mSdlWindow.Renderer());
    }
}

void Engine::EngineImplementation::ParseEvents()
{
    SDL_Event event;
    mMouseButtonPressed = false;

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                mQuit = true;
                break;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_ESCAPE)
                {
                    mQuit = true;
                }
                if(event.key.keysym.sym < 256)
                {
                    mKeyStates[event.key.keysym.sym] = true;
                }
                break;
            case SDL_KEYUP:
                if(event.key.keysym.sym < 256)
                {
                    mKeyStates[event.key.keysym.sym] = false;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(!mMouseButtonDown)
                {
                    mMouseButtonPressed = true;
                }

                mMouseButtonDown = true;
                break;
            case SDL_MOUSEBUTTONUP:
                mMouseButtonDown = false;
                break;
            case SDL_MOUSEMOTION:
                mMouseX = static_cast<float>(event.motion.x);
                mMouseY = static_cast<float>(event.motion.y);
                break;
            default:
                break;
        }
    }
}
} // namespace King
