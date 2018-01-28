#ifndef GameObject_h
#define GameObject_h

#include "SDL_render.h"
#include <string>

class GameObject
{
public:
	GameObject();
	~GameObject();

	void setTexture(std::string path, SDL_Renderer* renderer);
	void updatePosition(int, int);
	void setAngle(float);

	SDL_Texture* texture() { return m_texture; }
	SDL_Rect rect() { return m_objRect; }
	float angle() { return m_angle; }

	void setX(int);
	void setY(int);

private:
	SDL_Texture * m_texture;
	SDL_Rect m_objRect;
	float m_angle;
};

#endif