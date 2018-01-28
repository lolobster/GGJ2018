#include "GameObject.h"
#include "SDL_image.h"


GameObject::GameObject() :
m_texture(NULL),
m_objRect(),
m_angle(0.0f)
{
	m_objRect.x = 0;
	m_objRect.y = 0;
}


GameObject::~GameObject()
{
	SDL_DestroyTexture(m_texture);
}

void GameObject::setTexture(std::string path, SDL_Renderer* renderer)
{
	//Load image at specified path
	m_texture = IMG_LoadTexture(renderer, path.c_str());
	if (m_texture == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		SDL_QueryTexture(m_texture, NULL, NULL, &m_objRect.w, &m_objRect.h);
	}
}

void GameObject::updatePosition(int newX, int newY)
{
	m_objRect.x += newX;
	m_objRect.y += newY;
}

void GameObject::setAngle(float angle)
{
	m_angle += angle;
}

void GameObject::setX(int pos)
{
	m_objRect.x = pos;
}

void GameObject::setY(int pos)
{
	m_objRect.y = pos;
}