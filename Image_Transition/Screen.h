#pragma once

#include <SDL.h>

struct ScreenDimensions
{
	int width;
	int height;
};

class Screen {
private:
	SDL_Window* window;
	SDL_GLContext ctx;

	Screen();
	Screen(const Screen&);
	Screen& operator=(const Screen&);

public:
	static Screen* Instance();
	ScreenDimensions getDims();
	bool Init();
	void Clear();
	void SwapBuf();
	void Close();
};