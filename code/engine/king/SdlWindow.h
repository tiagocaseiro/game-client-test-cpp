// (C) king.com Ltd 2021
#pragma once

#include <memory>

struct SDL_Window;
struct SDL_Renderer;

namespace King {
	class SdlWindow {
	public:
		SdlWindow(unsigned int width, unsigned int height);
		operator SDL_Window*();
		SDL_Renderer& Renderer();
		void Show();

	private:
		std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> mSDLWindow;
		std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> mRenderer;
	};
}
