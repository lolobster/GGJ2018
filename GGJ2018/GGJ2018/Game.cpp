#include "Game.h"
#include "SDL_image.h"

Game::Game() :
m_renderer(nullptr),
m_signalObjects(),
m_asteroids(),
m_bgTexture(nullptr),
m_earth(new GameObject),
m_moon(new GameObject),
m_alien(new GameObject),
m_textTexture(nullptr),
m_weaponTextTexture(nullptr),
m_text("Test"),
m_alienLines(),
m_inputTimer(),
FPS(60),
m_radius(0),
m_quit(false),
m_win(false)
{
	// Init window
	m_windowManager.init();

	// Init renderer
	m_renderer = SDL_CreateRenderer(m_windowManager.window(), -1, SDL_RENDERER_ACCELERATED);

	// Init text
	TTF_Init();
	m_font = TTF_OpenFont("../font/moonhouse.ttf", 36);

	// Add alien lines
	m_alienLines.push_back("Sacre bleu!");
	m_alienLines.push_back("Scheiße!");
	m_alienLines.push_back("Not again!");
	m_alienLines.push_back("Perkele!");
	m_alienLines.push_back("#&@%#!@#%#!");
	m_alienLines.push_back("Zh68sd6gsg568dshg!");
	m_alienLines.push_back("XZJVCXBLDSGO!");
	m_alienLines.push_back("Aaaargh!");
	m_alienLines.push_back("Move over Jimmy, you suck at this!");
	m_alienLines.push_back("Stop backseating Harold, I can handle this!");
	m_alienLines.push_back("I fired and then I MYESSED!");


	int imgFlags = IMG_INIT_PNG;

	// Load background texture
	m_bgTexture = IMG_LoadTexture(m_renderer, "../textures/bg.png");
	if (m_bgTexture == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to load background image!");
	}

	// Load game object textures
	m_earth->setTexture("../textures/earth.png", m_renderer);
	m_earth->setX(m_windowManager.windowWidth() - m_earth->rect().w);
	m_earth->setY(m_windowManager.windowHeight() * 0.5f - m_earth->rect().h * 0.5f);

	m_moon->setTexture("../textures/moon.png", m_renderer);
	m_moon->setX(m_windowManager.windowWidth() - m_earth->rect().w * 1.75f);
	m_moon->setY(m_windowManager.windowHeight() * 0.5f - m_earth->rect().h * 0.5f);

	SDL_Point moonCenter = { m_moon->rect().x + m_moon->rect().w * 0.5,
		m_moon->rect().y - m_moon->rect().h * 0.5 };
	SDL_Point earthCenter = { m_earth->rect().x + m_earth->rect().w * 0.5,
		m_earth->rect().y + m_earth->rect().h * 0.5 };
	int distX = (earthCenter.x - moonCenter.x) * (earthCenter.x - moonCenter.x);
	int distY = (earthCenter.y - moonCenter.y) * (earthCenter.y - moonCenter.y);

	m_radius = sqrt(distX + distY);

	int asteroidRad = m_earth->rect().w * 2.75f;
	for (int i = 0; i < 6; ++i)
	{
		GameObject *asteroid = new GameObject();
		asteroid->setTexture("../textures/asteroid.png", m_renderer);
		asteroid->setX(m_windowManager.windowWidth() - asteroidRad);
		asteroid->setY(m_windowManager.windowHeight() * 0.5f - m_earth->rect().h * 0.5f);
		
		m_asteroids.push_back(asteroid);

		asteroidRad += 0.5f;
	}

	m_alien->setTexture("../textures/alien.png", m_renderer);
	m_alien->setY(m_windowManager.windowHeight() * 0.5f - m_alien->rect().h * 0.5f);
}

Game::~Game()
{

	delete m_alien;
	delete m_earth;
	delete m_moon;

	// Asteroids
	for (int i = 0; i < m_asteroids.size(); ++i)
	{
		delete m_asteroids.at(i);
	}
	m_asteroids.clear();

	// Signals
	for (int j = 0; j < m_signalObjects.size(); ++j)
	{
		delete m_signalObjects.at(j);
	}
	m_signalObjects.clear();

	// Text
	TTF_CloseFont(m_font);
	SDL_DestroyTexture(m_textTexture);
	SDL_DestroyTexture(m_weaponTextTexture);
	TTF_Quit();


	// Renderer
	SDL_DestroyRenderer(m_renderer);

	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Finished destructing game objects");
}


void Game::run()
{
	Timer capTimer;
	Timer fpsTimer;
	int countedFrames = 0;

	m_inputTimer.start();
	fpsTimer.start();

	while (!m_quit)
	{
		capTimer.start();

		// Clear screen
		SDL_RenderClear(m_renderer);

		// Draw new stuff
		update();

		//Calculate and correct fps
		float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
		if (avgFPS > 2000000)
		{
			avgFPS = 0;
		}

		++countedFrames;
		//If frame finished early
		int frameTicks = capTimer.getTicks();
		if (frameTicks < 1000 / FPS)
		{
			//Wait remaining time
			SDL_Delay(1000 /FPS - frameTicks);
		}
	}
}

void Game::update()
{
	// Update all the things
	// EARTH
	m_earth->setAngle(0.25f);
	
	// MOON
	m_moon->setAngle(0.5f);
	SDL_Point moonCenter = { m_moon->rect().x + m_moon->rect().w * 0.5,
							 m_moon->rect().y - m_moon->rect().h * 0.5 };
	SDL_Point earthCenter = { m_earth->rect().x + m_earth->rect().w * 0.5,
							  m_earth->rect().y + m_earth->rect().h * 0.5 };

	float radian = (m_moon->angle()*2 * M_PI) / 180;

	float deltaX = earthCenter.x - (m_radius * cos(radian));
	float deltaY = earthCenter.y - (m_radius * sin(radian));
	
 	m_moon->setX(deltaX);
	m_moon->setY(deltaY);

	for (int ast = 0; ast < m_asteroids.size(); ++ast)
	{
		GameObject *asteroid = m_asteroids.at(ast);
	
		asteroid->setAngle(0.25f + ast * 0.5f);

		SDL_Point asteroidCenter = { asteroid->rect().x + asteroid->rect().w * 0.5,
			asteroid->rect().y - asteroid->rect().h * 0.5 };

		float radian = (asteroid->angle() * 2 * M_PI) / 180;

		int asteroidRad = m_radius * 3 + ast * 50;

		float deltaX = earthCenter.x - (asteroidRad * cos(radian));
		float deltaY = earthCenter.y - (asteroidRad * sin(radian));

		asteroid->setX(deltaX);
		asteroid->setY(deltaY);
	}

	// Check for collisions
	if (m_signalObjects.size() > 0)
	{
		handleCollision();
	}

	// Draw all the things
	draw();

	// Do all the rest
	handleInput();
}

void Game::draw()
{
	// Draw BACKGROUND
	SDL_RenderCopy(m_renderer, m_bgTexture, NULL, NULL);

	// Draw SIGNALS
	if (m_signalObjects.size() > 0)
	{
		for (int i = 0; i < m_signalObjects.size(); ++i)
		{
			SDL_RenderCopy(m_renderer, m_signalObjects.at(i)->texture(), NULL, &m_signalObjects.at(i)->rect());
		}
	}

	// Draw ALIEN
	SDL_RenderCopy(m_renderer, m_alien->texture(), NULL, &m_alien->rect());

	// Draw EARTH
	SDL_Point earthCenter = { m_earth->rect().w * 0.5, m_earth->rect().h* 0.5 };
	SDL_RenderCopyEx(m_renderer, m_earth->texture(), NULL, &m_earth->rect(), m_earth->angle(), &earthCenter, SDL_FLIP_NONE);

	// Move, rotate and draw MOON
	SDL_Point moonCenter = { m_moon->rect().w * 0.5, m_moon->rect().h* 0.5 };
	SDL_RenderCopyEx(m_renderer, m_moon->texture(), NULL, &m_moon->rect(), m_moon->angle(), &moonCenter, SDL_FLIP_NONE);

	// Move, rotate and draw ASTEROIDS
	for (int j = 0; j < m_asteroids.size(); ++j)
	{
		GameObject *asteroid = m_asteroids.at(j);
		SDL_Point asteroidCenter = { asteroid->rect().w * 0.5, asteroid->rect().h* 0.5 };
		SDL_RenderCopyEx(m_renderer, asteroid->texture(), NULL, &asteroid->rect(), asteroid->angle(), &asteroidCenter, SDL_FLIP_NONE);
	}

	// Draw text
	SDL_Rect textRect;
	SDL_QueryTexture(m_textTexture, NULL, NULL, &textRect.w, &textRect.h);
	textRect.x = m_alien->rect().x;
	textRect.y = m_alien->rect().y - m_alien->rect().h - textRect.h;
	SDL_RenderCopy(m_renderer, m_textTexture, NULL, &textRect);

	SDL_Rect weaponTextRect;
	SDL_QueryTexture(m_weaponTextTexture, NULL, NULL, &weaponTextRect.w, &weaponTextRect.h);
	weaponTextRect.x = m_windowManager.windowWidth() * 0.5 - weaponTextRect.w * 0.5;
	weaponTextRect.y = m_windowManager.windowHeight() - weaponTextRect.h;
	if (m_inputTimer.getTicks() >= 3500)
	{
		SDL_Color col = { 0, 255, 0 };
		SDL_Surface* textSurface = TTF_RenderText_Solid(m_font, "Ready for transmission!", col);
		m_weaponTextTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
		SDL_FreeSurface(textSurface);
		SDL_RenderCopy(m_renderer, m_weaponTextTexture, NULL, &weaponTextRect);
	}
	else
	{
		SDL_Color col = { 255, 0, 0 };
		SDL_Surface* textSurface = TTF_RenderText_Solid(m_font, "Preparing transmitter", col);
		m_weaponTextTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
		SDL_FreeSurface(textSurface);
		SDL_RenderCopy(m_renderer, m_weaponTextTexture, NULL, &weaponTextRect);
	}

	// Update screen
	SDL_RenderPresent(m_renderer);
}

void Game::handleInput()
{
	SDL_Event event;

	while (SDL_PollEvent(&event) != 0)
	{
		if (event.type == SDL_QUIT)
		{
			m_quit = true;
			SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Quit");
		}
		else
		{
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
				{
					m_quit = true;
					SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Quit");
					break;
				}
				case SDLK_SPACE:
				{
					if (m_inputTimer.getTicks() >= 3500)
					{
						SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "New signal");

						GameObject *signal = new GameObject();
						signal->setTexture("../textures/signal.png", m_renderer);
						signal->setX(0 + m_alien->rect().w * 1.25f);
						signal->setY(m_windowManager.windowHeight() * 0.5f - signal->rect().h * 0.5f);

						m_signalObjects.push_back(signal);
						m_inputTimer.stop();
						m_inputTimer.start();
					}
					break;
				}
				}
			}
		}
	}
}

void Game::handleCollision()
{
	SDL_Color color = { 255, 255, 255 };

	for (int i = 0; i < m_signalObjects.size(); ++i)
	{
		GameObject *signal = m_signalObjects.at(i);

		signal->updatePosition(3, 0);

		if (signal->rect().x + signal->rect().w >= m_earth->rect().x)
		{
			// Vicory?
			SDL_Log("Collision: EARTH");
			// Delete last SIGNAL object
			delete signal;
			m_signalObjects.erase(m_signalObjects.begin() + i);
		}
		// Horrible
		else if (signal->rect().x + signal->rect().w >= m_moon->rect().x &&
			signal->rect().x + signal->rect().w <= m_moon->rect().x + m_moon->rect().w &&
			signal->rect().y - signal->rect().h * 0.5 >= m_moon->rect().y &&
			signal->rect().y - signal->rect().h * 0.5 <= m_moon->rect().y + m_moon->rect().h)
		{
			// Fail
			SDL_Log("Collision: MOON");
			// Delete last SIGNAL object
			delete signal;
			m_signalObjects.erase(m_signalObjects.begin() + i);

			int index = rand() % m_alienLines.size();
			m_text = m_alienLines.at(index);
			SDL_Surface* textSurface = TTF_RenderText_Solid(m_font, m_text.c_str(), color);
			m_textTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
			SDL_FreeSurface(textSurface);
		}
		else
		{
			for (int aster = 0; aster < m_asteroids.size(); ++aster)
			{
				GameObject *asteroid = m_asteroids.at(aster);

				if (signal->rect().x + signal->rect().w >= asteroid->rect().x &&
					signal->rect().x + signal->rect().w <= asteroid->rect().x + asteroid->rect().w &&
					signal->rect().y - signal->rect().h * 0.5 >= asteroid->rect().y &&
					signal->rect().y - signal->rect().h * 0.5 <= asteroid->rect().y + asteroid->rect().h)
				{
					// Fail
					SDL_Log("Collision: ASTEROID");
					// Delete last SIGNAL object
					delete signal;
					m_signalObjects.erase(m_signalObjects.begin() + i);

					int index = rand() % m_alienLines.size();
					m_text = m_alienLines.at(index);
					SDL_Surface* textSurface = TTF_RenderText_Solid(m_font, m_text.c_str(), color);
					m_textTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);
					SDL_FreeSurface(textSurface);
				}
			}
		}
	}


	// TODO Check if hit correct spot


}