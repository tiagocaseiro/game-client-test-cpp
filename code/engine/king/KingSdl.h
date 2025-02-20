// (C) king.com Ltd 2021
#pragma once

namespace King {
	class Sdl {
	public:
		Sdl(int flags);
		~Sdl();
	private:
		Sdl(const Sdl&) = delete;
	};
}
