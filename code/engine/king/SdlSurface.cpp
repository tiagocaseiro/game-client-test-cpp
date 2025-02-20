// (C) king.com Ltd 2021
#include "SdlSurface.h"

#include <stdexcept>
#include <string>
#include <glew/glew.h>
#include <SDL.h>
#include <SDL_image.h>

#include "SdlWindow.h"

namespace King {
	SdlSurface::SdlSurface(const char* filename, SDL_Renderer& renderer)
		: mSurface(IMG_Load(filename), SDL_FreeSurface)
		, mTexture(SDL_CreateTextureFromSurface(&renderer, &*mSurface), SDL_DestroyTexture) {
	}

	void SdlSurface::Blit(SdlSurface& target, int x, int y) {
		SDL_Rect rect;
		rect.x = x;
		rect.y = y;
		rect.w = mSurface->w;
		rect.h = mSurface->h;

		SDL_BlitSurface(&*mSurface, nullptr, &*target.mSurface, nullptr);
	}

	void SdlSurface::Blit(SdlWindow& window, float x, float y, float rotation, float scale) {
		SDL_Rect srcRect;
		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = mSurface->w;
		srcRect.h = mSurface->h;

		SDL_Rect dstRect;
		dstRect.x = int(x);
		dstRect.y = int(y);
		dstRect.w = int(mSurface->w * scale);
		dstRect.h = int(mSurface->h * scale);

		SDL_Point center;
		center.x = int(mSurface->w / 2.0f);
		center.y = int(mSurface->h / 2.0f);

		SDL_RenderCopyEx(&window.Renderer(),
						 &*mTexture,
						 &srcRect,
						 &dstRect,
						 static_cast<double>(rotation),
						 &center,
						 SDL_RendererFlip::SDL_FLIP_NONE);
	}

	void SdlSurface::BlitSub(SdlWindow& window, const Rect& source, const Rect& target) {
		SDL_Rect srcRect;
		srcRect.x = source.x;
		srcRect.y = source.y;
		srcRect.w = source.w;
		srcRect.h = source.h;

		SDL_Rect dstRect;
		dstRect.x = target.x;
		dstRect.y = target.y;
		dstRect.w = target.w;
		dstRect.h = target.h;

		SDL_Point center;
		center.x = int(mSurface->w / 2.0f);
		center.y = int(mSurface->h / 2.0f);

		SDL_RenderCopyEx(&window.Renderer(),
						 &*mTexture,
						 &srcRect,
						 &dstRect,
						 0.0,
						 &center,
						 SDL_RendererFlip::SDL_FLIP_NONE);
	}

	int SdlSurface::Width() const {
		return mSurface->w;
	}

	int SdlSurface::Height() const {
		return mSurface->h;
	}

	SdlSurface::operator SDL_Surface*() {
		return mSurface.get();
	}
}
