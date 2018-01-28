#include "WindowManager.h"


WindowManager::WindowManager() :
m_window(nullptr),
m_screenSurface(nullptr),
m_screenWidth(1200),
m_screenHeight(900)
{
}


WindowManager::~WindowManager()
{
	SDL_DestroyWindow(m_window);

	SDL_Quit();
}

void WindowManager::init()
{
	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL, "SDL init failed!");
	}
	else
	{
		// Create window
		m_window = SDL_CreateWindow("They might be aliens", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, m_screenWidth, m_screenHeight, SDL_WINDOW_SHOWN);
		if (m_window == nullptr)
		{
			SDL_LogMessage(SDL_LOG_CATEGORY_ERROR, SDL_LOG_PRIORITY_CRITICAL, "SDL window could not be created!");
		}
		else
		{
			m_screenSurface = SDL_GetWindowSurface(m_window);
		}
	}
}