#ifndef WindowManager_h
#define WindowManager_h

#include "SDL.h"

class WindowManager
{
public:
	WindowManager();
	~WindowManager();

	void init();

	SDL_Window* window() { return m_window; }
	SDL_Surface* screenSurface() { return m_screenSurface; }

	const int windowWidth() { return m_screenWidth; }
	const int windowHeight() { return m_screenHeight; }

private:
	SDL_Window *m_window;
	SDL_Surface *m_screenSurface;
	const int m_screenWidth;
	const int m_screenHeight;
};

#endif