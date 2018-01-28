#ifndef Game_h
#define Game_h

#include "SDL.h"
#include "SDL_ttf.h"
#include "WindowManager.h"
#include "GameObject.h"
#include "Timer.h"

#include <vector>
#include <string>

class Game
{
public:
	Game();
	~Game();

	void run();
	void update();
	void draw();

	void handleInput();
	void handleCollision();

private:
	// Create window
	WindowManager m_windowManager;
	SDL_Renderer *m_renderer;

	SDL_Texture *m_bgTexture;
	SDL_Texture *m_winTexture;
	
	std::vector<GameObject*> m_signalObjects;
	std::vector<GameObject*> m_asteroids;
	GameObject *m_earth;
	GameObject *m_moon;
	GameObject *m_alien;

	// Text
	TTF_Font *m_font;
	SDL_Texture *m_textTexture;
	SDL_Texture *m_weaponTextTexture;
	std::string m_text;
	std::vector<std::string> m_alienLines;

	Timer m_inputTimer;

	const int FPS;
	double m_radius;

	bool m_quit;
	bool m_win;
};

#endif