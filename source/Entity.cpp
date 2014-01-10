#include "..\header\Entity.h"


Entity::Entity() : 
  speed(100.f) //100px/second
{
}


Entity::~Entity()
{
}

void Entity::move(const sf::Vector2f displacement)
{
	setPosition(getPosition() + displacement);
}


