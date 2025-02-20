// (C) king.com Ltd 2021
#include "SdlWindow.h"

#include <stdexcept>
#include <string>
#include <SDL.h>
#include "SdlSurface.h"

namespace King {
	SdlWindow::SdlWindow(unsigned int width, unsigned int height)
		: mSDLWindow(SDL_CreateWindow("KingTest"
		, SDL_WINDOWPOS_UNDEFINED
		, SDL_WINDOWPOS_UNDEFINED
		, width
		, height
		, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL), SDL_DestroyWindow)
		, mRenderer(SDL_CreateRenderer(mSDLWindow.get(), -1, SDL_RENDERER_ACCELERATED), SDL_DestroyRenderer) {
		if (!mSDLWindow) {
			throw std::runtime_error(std::string("Error creating window: ") + SDL_GetError());
		}
	}
	void SdlWindow::Show() {
		SDL_ShowWindow(mSDLWindow.get());
	}
	SdlWindow::operator SDL_Window*() {
		return mSDLWindow.get();
	}
	SDL_Renderer& SdlWindow::Renderer() {
		return *mRenderer;
	}
}
