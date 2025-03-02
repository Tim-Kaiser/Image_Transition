#pragma once

#include <SDL.h>

class Screen {
private:
	SDL_Window* window;
	SDL_GLContext ctx;

	Screen();
	Screen(const Screen&);
	Screen& operator=(const Screen&);

public:
	static Screen* Instance();
	bool Init();
	void Clear();
	void SwapBuf();
	void Close();
};